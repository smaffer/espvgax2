#include "ESPVGAX2.h"
#include <math.h>

volatile uint32_t ESPVGAX2_ALIGN32 ESPVGAX2::fbw[ESPVGAX2_HEIGHT][ESPVGAX2_WWIDTH];

static volatile uint32_t *line;
static volatile uint32_t fby;
static uint32_t rby, yac;
static uint32_t vsync;
static uint32_t running;
static uint32_t wcnt;

volatile uint8_t *ESPVGAX2::fbb=(volatile uint8_t*)&ESPVGAX2::fbw[0];

// wait a fixed numbers of CPU cycles
#define NOP_DELAY_(N) asm(".rept " #N "\n\t nop \n\t .endr \n\t":::)
#define NOP_DELAY(N) NOP_DELAY_(N)

void 
#if F_CPU==80000000L //bs segment does not fit for 160MHZ version
ICACHE_RAM_ATTR 
#endif
vga_handler() {
  //noInterrupts(); 
  
  //begin negative HSYNC
  GPOC=1<<ESPVGAX2_HSYNC_PIN;

  #if F_CPU==80000000L
  NOP_DELAY(160); //2us*80MHz
  #else
  NOP_DELAY(320); //2us*160MHz
  #endif
  //end negative HSYNC
  GPOS=1<<ESPVGAX2_HSYNC_PIN;

  //begin or end VSYNC, depending of value of vsync variable
  ESP8266_REG(vsync)=1<<ESPVGAX2_VSYNC_PIN;

  //write PIXELDATA
  if (running && line) { 
    register volatile uint32_t *out = (volatile uint32_t *)0x60000300; //GPO
    register uint32_t *in = (uint32_t*)line;
    register uint32_t gpo0 = GPO & 0xffff0fff;
    register uint32_t c;

    //wait some time to align horizontal data
    #if F_CPU==80000000L
    NOP_DELAY(160);  
    #else
    NOP_DELAY(360);  
    #endif
    /*
     * PIN   D8  D5  D7  D6   ?   ?   ?   ?   ?   ?  D1  D2  D9  D4 D10  D3
     * GPIO  15  14  13  12  11  10   9   8   7   6   5   4   3   2   1   0
     * USE    X   B   G   R   -   -   -   -   -   -   V   H   -   -   -   -
     */
    #define PUT8PIXELS \
      c = *in++; \
      *out = ((c << 12U) & 0x0000f000U) | gpo0; \
      *out = ((c <<  8U) & 0x0000f000U) | gpo0; \
      *out = ((c <<  4U) & 0x0000f000U) | gpo0; \
      *out = ((c <<  0U) & 0x0000f000U) | gpo0; \
      *out = ((c >>  4U) & 0x0000f000U) | gpo0; \
      *out = ((c >>  8U) & 0x0000f000U) | gpo0; \
      *out = ((c >> 12U) & 0x0000f000U) | gpo0; \
      *out = ((c >> 16U) & 0x0000f000U) | gpo0;
    
    #define PUT64PIXELS \
      PUT8PIXELS; PUT8PIXELS; PUT8PIXELS; PUT8PIXELS; \
      PUT8PIXELS; PUT8PIXELS; PUT8PIXELS; PUT8PIXELS;

    PUT64PIXELS;
    PUT64PIXELS;
    PUT64PIXELS;
    PUT64PIXELS;
    
    #if F_CPU==80000000L //80Mhz: send 280px
    //PUT8PIXELS;
    #else //160Mhz: send 320px
    PUT64PIXELS;
    //PUT8PIXELS;
    //PUT8PIXELS;
    #endif
    
    *out = gpo0;
  }
  // prepare for the next vga_handler run
  fby++;
  switch (fby) {
  case 525: 
    // restart from the beginning
    fby=0;
    rby=0;
    yac=0; 
    /* 
     * feed the dog. keep ESP8266 WATCHDOG awake. VGA signal generation works 
     * well if there are ZERO calls to Arduino functions like delay or yield. 
     * These functions will perform many background tasks that generates some
     * delays on the VGA signal stability but keeps the hardware WATCHDOG awake. 
     * I have not figured out why this happen, probably there are some hardware
     * task that generate a jitter in the interrupt callback, like on ATMEGA MCU,
     * see the VGAX dejitter nightmare
     */
    if (0==(wcnt++%60)) //feed every 60 frames
      wdt_reset();
    break;
  case 490: 
    // next line will begin negative VSYNC 
    vsync=0x308; 
    break;
  case 492: 
    // next line will end negative VSYNC
    vsync=0x304; 
    break;
  };
  // fetch the next line, or empty line in case of VGA lines [480..524]
  line=(rby<ESPVGAX2_HEIGHT) ? ESPVGAX2::fbw[rby] : NULL;
  if (yac%2) 
    rby++;  
  yac++;
}
void ESPVGAX2::begin() {
  pinMode(ESPVGAX2_VSYNC_PIN, OUTPUT);
  pinMode(ESPVGAX2_HSYNC_PIN, OUTPUT);
  pinMode(ESPVGAX2_COLOR0_PIN, OUTPUT);
  pinMode(ESPVGAX2_COLOR1_PIN, OUTPUT);
  pinMode(ESPVGAX2_COLOR2_PIN, OUTPUT);
  pinMode(ESPVGAX2_COLOR3_PIN, OUTPUT);
  // prepare first line
  fby=0;
  line=fbw[0];
  // begin with positive VSYNC
  vsync=0x304;
  running=1;
  // install vga_handler interrupt
  noInterrupts();
  timer1_disable();
  timer1_attachInterrupt(vga_handler);
  timer1_isr_init();
  timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
  timer1_write(10); 
  //timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  //timer1_write(US_TO_RTC_TIMER_TICKS(32));
  interrupts();
  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
}
void ESPVGAX2::pause() {
  running=0;
}
void ESPVGAX2::resume() {
  running=1;
}
void ESPVGAX2::end() {
  // disable installed interrupt
  noInterrupts();
  timer1_detachInterrupt();
  interrupts();
}
static inline uint32_t getTicks() {
  uint32_t ccount;
  asm volatile ("rsr %0, ccount":"=a"(ccount));
  return ccount;
}
void ESPVGAX2::delay(uint32_t msec) {
  // predict the CPU ticks to be awaited
  uint32_t us=msec*1000;
  uint32_t start=getTicks();
  uint32_t target=start+us*
    #if F_CPU==80000000L
    80
    #else
    160
    #endif
    ;
  uint32_t prev=start;
  int overflow=0;
  for (;;) {
    uint32_t now=getTicks();
    if (target<start) {
      // overflow will occur
      if (prev>now)
        // overflow is occurred
        overflow=1;
      else if (overflow && now>target)
        // end is reached
        break;
    } else if (now>target) {
      // end is reached
      break;
    }
    prev=now;
  }
}
static uint64_t rand_next=1;

uint32_t ESPVGAX2::rand() {
  rand_next = rand_next * 1103515245ULL + 12345;
  return rand_next+((uint32_t)(rand_next / 65536) % 32768);
}
void ESPVGAX2::srand(unsigned int seed) {
  rand_next = seed;
}
void ESPVGAX2::waitVSync() {
  while (fby<ESPVGAX2_HEIGHT)
    ;
}
//include blit methods, implemented via a bunch of macros
#include "espvgax2_blit.h"

//include print methods, implemented via a bunch of macros
#include "espvgax2_print.h"

//include draw primitives methods
#include "espvgax2_draw.h"
