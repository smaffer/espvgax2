#include <ESPVGAX2.h>

ESPVGAX2 vga;

void loop() {
  while (1) { 
    //never return from loop(). never call delay() or yield() functions
  }
}
void setup() {
  vga.begin();
  vga.drawRect(0, 0, ESPVGAX2_WIDTH-1, ESPVGAX2_HEIGHT-1, 0x2, true);
  vga.drawRect(0, 0, ESPVGAX2_WIDTH-1, ESPVGAX2_HEIGHT-1, 0xb, false);

  vga.drawCircle(ESPVGAX2_WIDTH/2, ESPVGAX2_HEIGHT/2, 120-1, 0xf, false);
  vga.drawCircle(ESPVGAX2_WIDTH/2, ESPVGAX2_HEIGHT/2, 60-1, 0x3, true);
  vga.drawCircle(ESPVGAX2_WIDTH/2, ESPVGAX2_HEIGHT/2, 30-1, 0x1, false);
  
  int sw=32, sh=28;
  vga.drawRect(10, 10, sw, sh, 0xa, true, ESPVGAX2_OP_XOR);
  vga.drawRect(ESPVGAX2_WIDTH-sw-10, 10, sw, sh, 0xa, true, ESPVGAX2_OP_XOR);
  vga.drawRect(10, ESPVGAX2_HEIGHT-sh-10, sw, sh, 0xa, true, ESPVGAX2_OP_XOR);
  vga.drawRect(ESPVGAX2_WIDTH-sw-10, ESPVGAX2_HEIGHT-sh-10, sw, sh, 0xa, true, ESPVGAX2_OP_XOR);

  vga.drawLine(0, 0, ESPVGAX2_WIDTH, ESPVGAX2_HEIGHT, 0xc, ESPVGAX2_OP_XOR);
  vga.drawLine(0, ESPVGAX2_HEIGHT, ESPVGAX2_WIDTH, 0, 0xc, ESPVGAX2_OP_XOR);
  vga.drawLine(ESPVGAX2_WIDTH/2, 0, ESPVGAX2_WIDTH/2, ESPVGAX2_HEIGHT, 0xc, ESPVGAX2_OP_XOR);
  vga.drawLine(0, ESPVGAX2_HEIGHT/2, ESPVGAX2_WIDTH, ESPVGAX2_HEIGHT/2, 0xc, ESPVGAX2_OP_XOR);
}
