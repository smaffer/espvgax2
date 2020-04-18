#include <ESPVGAX2.h>
#include <fonts2/monodos8.h>

ESPVGAX2 vga;

static const ESPVGAX2_ALIGN32 char str0[] PROGMEM="\
ESPVGAX2 Version 1.0\n\
\n\
\e\001Lorem ipsum \e\003dolor sit amet, \e\010consectetur adipiscing elit, \
\e\017\sed do eiusmod tempor \e\013incididunt ut \e\014labore et dolore \e\012\
magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco \
laboris nisi ut aliquip ex ea commodo consequat. \e\017\n\
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
dolore magnam aliquam quaerat voluptatem. ";

void loop() {
  while (1) { 
    //never return from loop(). never call delay() or yield() functions
  }
}
void setup() {
  vga.begin();
  vga.setBitmapFont((uint8_t*)img2_monodos8_data, 8);
  vga.print_P(str0, 0, 0, 0xf, true);
}