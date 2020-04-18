#include <ESPVGAX2.h>
#include <fonts2/monaco9.h>
#include <fonts2/mfont6.h>

ESPVGAX2 vga;

static const ESPVGAX2_ALIGN32 char str0[] PROGMEM="\
ESPVGAX2 Version 1.0\n\
\n\
Lorem ipsum \e\001dolor sit amet, \e\007consectetur adipiscing elit, sed do eiusmod tempor \
incididunt ut \e\010labore et dolore magna aliqua. \e\017Ut enim ad \e\013minim veniam, quis \
nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. \n\
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu \
fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in \
culpa qui officia deserunt mollit anim id est laborum.\n\
\n\
ESPVGAX2 2020 by Sandro Maffiodo\n";

void loop() {
  while (1) { 
    //never return from loop(). never call delay() or yield() functions
  }
}
void setup() {
  vga.begin();

  vga.setFont((uint8_t*)fnt2_monaco9_data, FNT2_MONACO9_SYMBOLS_COUNT, 
              FNT2_MONACO9_HEIGHT, FNT2_MONACO9_GLYPH_WIDTH);
  ESPVGAX2::PrintInfo end;
  end=vga.print_P(str0, 2, 2, 0xf, true);

  vga.setFont((uint8_t*)fnt2_mfont6_data, FNT2_MFONT6_SYMBOLS_COUNT, 
              FNT2_MFONT6_HEIGHT, FNT2_MFONT6_GLYPH_WIDTH);
  vga.print_P(str0, 2, end.y+10, 0xf, true, -1, ESPVGAX2_OP_OR, false);
}