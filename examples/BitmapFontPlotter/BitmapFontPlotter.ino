#include <ESPVGAX2.h>
#include <fonts2/monodos12.h>
#include <fonts2/monodos8.h>

ESPVGAX2 vga;

static const ESPVGAX2_ALIGN32 char str0[] PROGMEM="\
ESPVGAX2 Version 1.0\n\
\n\
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor \
incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis \
nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. \n\
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu \
fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in \
culpa qui officia deserunt mollit anim id est laborum.\n\
Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium \
doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore \
veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim \
ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia \
consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque \
porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, \
adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et \
dolore magnam aliquam quaerat voluptatem.\n\
\n\
ESPVGAX2 2020 by Sandro Maffiodo";

int maxlen;
void loop() {
  while (1) { 
    //never return from loop(). never call delay() or yield() functions
    vga.clear(0);
    ESPVGAX2::PrintInfo end;
    //plot with MONODOS12 font, height 12
    vga.setBitmapFont((uint8_t*)img2_monodos12_data, 12);
    end.x=end.y=0;
    for (int i=0; i!=maxlen; i++) {
      //plot single character, with automatic text wrap 
      int c = 1 + (vga.rand() % 15);
      end=vga.print_P(str0+i, end.x, end.y, c, true, 1, ESPVGAX2_OP_SET, false, 0);
      vga.delay(2);
    }
    vga.delay(2000);
    vga.clear(0);
    //plot with MONODOS8 font, height 8
    vga.setBitmapFont((uint8_t*)img2_monodos8_data, 8);
    end.x=end.y=0;
    for (int i=0; i!=maxlen; i++) {
      //plot single character, with automatic text wrap 
      int c = 1 + (vga.rand() % 15);
      end=vga.print_P(str0+i, end.x, end.y, c, true, 1, ESPVGAX2_OP_SET, false, 0);
      vga.delay(2);
    }
    vga.delay(2000);
  }
}
void setup() {
  maxlen=strlen(str0);
  vga.begin();
}