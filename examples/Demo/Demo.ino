#include <ESPVGAX2.h>
#include <fonts2/mfont6.h>
#include <fonts2/ufont6.h>
#include <fonts2/courier10.h>
#include <fonts2/arial12.h>
#include <fonts2/monodos12.h>
#include <fonts2/monodos8.h>
#include <images2/pirate256.h>
#include <images2/pirate320.h>
#include <images2/logo128.h>
#include <images2/logomario.h>

void test_setpixel() {
  int x=ESPVGAX2::rand()%ESPVGAX2_WIDTH;
  int y=ESPVGAX2::rand()%ESPVGAX2_HEIGHT;
  ESPVGAX2::setpixel(x, y, (x==0 || y==0 || x==ESPVGAX2_WIDTH-1 || y==ESPVGAX2_HEIGHT-1) ? 0xf : ESPVGAX2::rand()%16);
}
void test_setpixel32() {
  int x=ESPVGAX2::rand()%ESPVGAX2_WWIDTH;
  int y=ESPVGAX2::rand()%ESPVGAX2_HEIGHT;
  int c=ESPVGAX2::rand()%16;
  int full=c | (c<<4) | (c<<8) | (c<<12) | (c<<16) | (c<<20) | (c<<24) | (c<<28);
  ESPVGAX2::setpixel32(x, y, full);
}
void test_randomline() {
  int y=ESPVGAX2::rand()%ESPVGAX2_HEIGHT;
  int c=ESPVGAX2::rand()%16;
  ESPVGAX2::clearLine(y, c);
}
void test_clear() {
  ESPVGAX2::clear(ESPVGAX2::rand()%16);
}
void test_blit() {
  int x=-100+ESPVGAX2::rand()%(ESPVGAX2_WIDTH +200);
  int y=-100+ESPVGAX2::rand()%(ESPVGAX2_HEIGHT+200);
  ESPVGAX2::blit_P((uint8_t*)img2_logo128_data, x, y, IMG2_LOGO128_WIDTH, IMG2_LOGO128_HEIGHT);
}
void test_blitwmask() {
  int x=-100+ESPVGAX2::rand()%(ESPVGAX2_WIDTH +200);
  int y=-100+ESPVGAX2::rand()%(ESPVGAX2_HEIGHT+200);
  ESPVGAX2::blitwmask_P((uint8_t*)img2_logo128_data, x, y, IMG2_LOGO128_WIDTH, IMG2_LOGO128_HEIGHT, 0x8);
}
void test_blitwmask_mario(bool mask) {
  int x=-16+ESPVGAX2::rand()%(ESPVGAX2_WIDTH +32);
  int y=-16+ESPVGAX2::rand()%(ESPVGAX2_HEIGHT+32);
  if (mask)
    ESPVGAX2::blitwmask_P((uint8_t*)img2_logomario_data, x, y, IMG2_LOGOMARIO_WIDTH, IMG2_LOGOMARIO_HEIGHT, 0x8);
  else
    ESPVGAX2::blit_P((uint8_t*)img2_logomario_data, x, y, IMG2_LOGOMARIO_WIDTH, IMG2_LOGOMARIO_HEIGHT);
}
static const ESPVGAX2_ALIGN32 char str0[] PROGMEM="\
ESPVGAX2 Version 1.0\n\
\n\
Lorem ipsum dolor sit amet, \e\001consectetur \e\002adipiscing elit, \e\003sed do \e\004eiusmod tempor \
\e\005incididunt ut \e\006labore et \e\007dolore magna \e\010aliqua. Ut enim \e\0011ad minim veniam, \e\017quis \
nostrud exercitation ullamco laboris nisi ut aliquip \
ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu \
\e\012fugiat nulla \e\013pariatur. Excepteur \e\014sint occaecat \e\015cupidatat non \e\016proident, sunt \e\017in \
culpa qui officia deserunt mollit anim id est laborum.\n\
Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam \
rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. \
Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores \
eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, \
consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam \
quaerat voluptatem\n\
";
void test_print() {
  ESPVGAX2::PrintInfo end;
  ESPVGAX2::clear(0);
  ESPVGAX2::setFont((uint8_t*)fnt2_mfont6_data, FNT2_MFONT6_SYMBOLS_COUNT, FNT2_MFONT6_HEIGHT);
  end=ESPVGAX2::print_P(str0, 2, 2, 1+(ESPVGAX2::rand() % 15), true);
  ESPVGAX2::setFont((uint8_t*)fnt2_ufont6_data, FNT2_UFONT6_SYMBOLS_COUNT, FNT2_UFONT6_HEIGHT);
  ESPVGAX2::print_P(str0, 2, end.y+10, 1+(ESPVGAX2::rand() % 15), true);
  ESPVGAX2::delay(4000);
  ESPVGAX2::clear(0);
  ESPVGAX2::setFont((uint8_t*)fnt2_arial12_data, FNT2_ARIAL12_SYMBOLS_COUNT, FNT2_ARIAL12_HEIGHT, FNT2_ARIAL12_GLYPH_WIDTH);
  end=ESPVGAX2::print_P(str0, 2, 2, 1+(ESPVGAX2::rand() % 15), true, -1, ESPVGAX2_OP_OR);
  ESPVGAX2::print_P(str0, 2, end.y+10, 1+(ESPVGAX2::rand() % 15), true, -1, ESPVGAX2_OP_OR, true);
  ESPVGAX2::delay(4000);
  ESPVGAX2::clear(0);
  end=ESPVGAX2::print_P(str0, 2, 2, 1+(ESPVGAX2::rand() % 15), true, -1, ESPVGAX2_OP_XOR);
  ESPVGAX2::print_P(str0, 2, end.y+10, 1+(ESPVGAX2::rand() % 15), true, -1, ESPVGAX2_OP_XOR);
  ESPVGAX2::delay(4000);
  ESPVGAX2::clear(0);
  ESPVGAX2::setBitmapFont((uint8_t*)img2_monodos12_data, 12);
  end.x=end.y=0;
  int slen = strlen(str0);
  for (int i=0; i<slen; i++) {
    if (pgm_read_byte(str0 + i)=='\e') {
      i+=2;
    }
    end=ESPVGAX2::print_P(str0+i, end.x, end.y, i<20 ? 0xf : 1+(ESPVGAX2::rand() % 15), true, 1, ESPVGAX2_OP_SET, false, 0);
    ESPVGAX2::delay(5);
  }
  ESPVGAX2::delay(2000);
  ESPVGAX2::clear(0);
  ESPVGAX2::setBitmapFont((uint8_t*)img2_monodos8_data, 8);
  end.x=end.y=0;
  for (int i=0; i<slen; i++) {
    if (pgm_read_byte(str0 + i)=='\e') {
      i+=2;
    }
    end=ESPVGAX2::print_P(str0+i, end.x, end.y, i<20 ? 0xf : 1+(ESPVGAX2::rand() % 15), true, 1, ESPVGAX2_OP_SET, false, 0);
    ESPVGAX2::delay(5);
  }
  //ESPVGAX2::print_P(str0, 0, end.y+8, 1+(ESPVGAX2::rand() % 15), true);
  ESPVGAX2::delay(2000);
}
char matrix[60][64];
void test_print_RAM() {
  for (int i=0; i!=32; i++) {
    int x=ESPVGAX2::rand() % 64;
    int y=ESPVGAX2::rand() % 60;
    uint8_t c=(uint8_t)(ESPVGAX2::rand()%256);
    if (c==0)
      c=1;
    else if (c=='\n' || c=='\e')
      c=' ';
    matrix[y][x]=(char)c;
  }
  ESPVGAX2::setBitmapFont((uint8_t*)img2_monodos8_data, 8);
  ESPVGAX2::print((char*)matrix, 0, 0, 1+(ESPVGAX2::rand() % 15), true, sizeof matrix);
}
void test_draw() {
  ESPVGAX2::clear(0);
  for (int i=0; i!=1000; i++) {
    int x=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    int radius=3+(ESPVGAX2::rand() % 120);
    ESPVGAX2::drawCircle(x-30, y-30, radius, 1+(ESPVGAX2::rand() % 15), false, ESPVGAX2_OP_XOR);
  }  
  ESPVGAX2::clear(0);
  for (int i=0; i!=1000; i++) {
    int x=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    int w=2+(ESPVGAX2::rand() % 120);
    int h=2+(ESPVGAX2::rand() % 120);
    ESPVGAX2::drawRect(x-30, y-30, w, h, 1+(ESPVGAX2::rand() % 15), false, ESPVGAX2_OP_XOR);
  }
  ESPVGAX2::clear(0);
  for (int i=0; i!=1000; i++) {
    int x0=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y0=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    int x1=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y1=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    ESPVGAX2::drawRect(x0-30, y0-30, x1, y1, 1+(ESPVGAX2::rand() % 15), false, ESPVGAX2_OP_XOR);
  }
  ESPVGAX2::clear(0);
  for (int i=0; i!=1000; i++) {
    int x=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    int radius=3+(ESPVGAX2::rand() % 120);
    ESPVGAX2::drawCircle(x-30, y-30, radius, 1+(ESPVGAX2::rand() % 15), true, ESPVGAX2_OP_SET);
  }
  ESPVGAX2::clear(0);
  for (int i=0; i!=1000; i++) {
    int x=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    int w=2+(ESPVGAX2::rand() % 120);
    int h=2+(ESPVGAX2::rand() % 120);
    ESPVGAX2::drawRect(x-30, y-30, w, h, 1+(ESPVGAX2::rand() % 15), true, ESPVGAX2_OP_SET);
  }
  ESPVGAX2::clear(0);
  for (int i=0; i!=512; i++) {
    int x0=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y0=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    int x1=ESPVGAX2::rand() % ESPVGAX2_WIDTH;
    int y1=ESPVGAX2::rand() % ESPVGAX2_HEIGHT;
    ESPVGAX2::drawRect(x0-30, y0-30, x1, y1, 1+(ESPVGAX2::rand() % 15), true, ESPVGAX2_OP_XOR);
  }
}
void loop() {
  while (1) { //never return from loop() or yield() will be called.
    #if ESPVGAX2_WIDTH==320
    ESPVGAX2::copy_P((uint32_t*)img2_pirate320_data);
    #else
    ESPVGAX2::copy_P((uint32_t*)img2_pirate256_data);
    #endif
    ESPVGAX2::delay(10000);
    
    for (int k=0; k!=99000; k++)
      test_setpixel();
    for (int k=0; k!=99000; k++)
      test_setpixel32();
    for (int k=0; k!=8000; k++)
      test_clear();
    for (int k=0; k!=99999; k++)
      test_randomline();
    ESPVGAX2::clear(0);
    ESPVGAX2::delay(1000);
    for (int k=0; k!=200; k++)
      test_blit();
    ESPVGAX2::clear(0);
    for (int k=0; k!=200; k++)
      test_blitwmask();
    ESPVGAX2::clear(0);
    for (int k=0; k!=2000; k++)
      test_blitwmask_mario(false);
    ESPVGAX2::clear(0);
    for (int k=0; k!=3000; k++)
      test_blitwmask_mario(true);
    ESPVGAX2::clear(0);
    for (int k=0; k!=1; k++)
      test_print();
    ESPVGAX2::clear(0);
    /* seem to create some memory error
    memset(matrix, ' ', sizeof matrix);
    for (int k=0; k!=100; k++)
      test_print_RAM();
    */
    ESPVGAX2::clear(0);
    for (int k=0; k!=1; k++)
      test_draw();     
    //yield(); //note! never call yield() or vga signal will flicker
    //delay(1000); //note! never call delay() or vga signal will flicker. if possible, use espvgax2.delay instead
  }
}
void setup() {      
  Serial.begin(9600);  
  ESPVGAX2::begin();
}