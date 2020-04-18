#include <ESPVGAX2.h>

ESPVGAX2 vga;

//set one pixel at a time
void random_setpixel() {
  vga.clear(0);
  for (int i=0; i!=99999; i++) {
    int x=vga.rand()%ESPVGAX2_WIDTH;
    int y=vga.rand()%ESPVGAX2_HEIGHT;
    vga.setpixel(x, y, vga.rand()%16);
  }
}
//set 8 pixels at a time
void random_setpixel32() {
  vga.clear(0);
  for (int i=0; i!=99999; i++) {
    int x=vga.rand()%ESPVGAX2_WWIDTH;
    int y=vga.rand()%ESPVGAX2_HEIGHT;
    vga.setpixel32(x, y, vga.rand());
  }
}
void loop() {
  while (1) { 
    //never return from loop(). never call delay() or yield() functions
    random_setpixel();
    random_setpixel32();
  }
}
void setup() {
  vga.begin();
}