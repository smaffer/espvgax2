#include <ESPVGAX2.h>
#include <images2/pirate256.h>
#include <images2/pirate320.h>

ESPVGAX2 vga;

void loop() {
  while (1) { 
  	//never return from loop(). never call delay() or yield() functions
  }
}
void setup() {      
  vga.begin();
  #if ESPVGAX2_WIDTH==320
  vga.blit_P((uint8_t*)img2_pirate320_data, 0, 0, IMG2_PIRATE320_WIDTH, IMG2_PIRATE320_HEIGHT);
  #else
  vga.blit_P((uint8_t*)img2_pirate256_data, 0, 0, IMG2_PIRATE256_WIDTH, IMG2_PIRATE256_HEIGHT);
  #endif
}

