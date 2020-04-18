//file included from ESPVGAX2.cpp

static ESPVGAX2_PROGMEM uint8_t *fnt=0; 
static uint16_t glyphscount;
static uint8_t fntheight;
static uint8_t hspace;
static uint8_t vspace;
static uint8_t fntglyphw;
static bool bmpfont;
static bool bmpcolor;

void ESPVGAX2::setFont(ESPVGAX2_PROGMEM uint8_t *fnt_, uint8_t glyphscount_, 
  uint8_t fntheight_, uint8_t fntglyphw_, uint8_t hspace_, uint8_t vspace_) {

  fnt=fnt_;
  glyphscount=glyphscount_;
  fntheight=fntheight_;
  bmpfont=false;
  hspace=hspace_;
  vspace=vspace_;
  fntglyphw=fntglyphw_;
}
void ESPVGAX2::setBitmapFont(ESPVGAX2_PROGMEM uint8_t *bmp, uint8_t h, int gw) {
  fnt=bmp;
  glyphscount=256;
  fntheight=h;
  bmpfont=true;
  hspace=0;
  vspace=0;
  fntglyphw=gw;
  bmpcolor=false;
}
void ESPVGAX2::setBitmapFontColor(ESPVGAX2_PROGMEM uint8_t *bmp, uint8_t h, int gw) {
  setBitmapFont(bmp, h, gw);
  bmpcolor=true;
}
#define PRINT_LOOP_CODE(reader) \
  int cr_dx0=dx0override!=-1 ? dx0override : dx0; \
  if (dy0>=ESPVGAX2_HEIGHT) \
    calc=true; \
  if (c=='\n') { \
    dx=cr_dx0; \
    dy0+=fntheight+vspace; \
  } else if (c=='\e') { \
    color=reader(pstr++); \
    if (color==0xff && 0) \
      color=default_color; \
  } else { \
    if (bmpfont) { \
      if (wrap && dx+8*fntglyphw>ESPVGAX2_WIDTH) { \
        dx=cr_dx0; \
        dy0+=fntheight; \
      } \
      uint8_t uc=0xff&((uint8_t)c); \
      int bx=(uc%16); \
      int by=fntheight*(uc/16); \
      if (!calc) { \
        if (bmpcolor) { \
          blit_P(fnt+(by*16+bx)*fntglyphw, \
                 dx, dy0, \
                 8*fntglyphw, \
                 fntheight, \
                 op, \
                 128*fntglyphw); \
        } else { \
          bitblit_P(fnt+(by*16+bx)*fntglyphw, \
                    dx, dy0, \
                    8*fntglyphw, \
                    fntheight, \
                    color, \
                    op, \
                    128*fntglyphw); \
        } \
      } \
      dx+=8*fntglyphw; \
    } else { \
      c-=33; \
      if (c>=0 && c<glyphscount) { \
        /* calculate glyph bitmap address */ \
        uint8_t *fntg=fnt+c*(4+fntheight*fntglyphw); \
        /* read glyph width (first byte) */ \
        uint8_t fntw=pgm_read_byte(fntg); \
        if (wrap && dx+fntw>ESPVGAX2_WIDTH) { \
          dx=cr_dx0; \
          dy0+=fntheight+vspace; \
        } \
        if (!calc) { \
          bitblit_P(fntg+4, \
                    dx, dy0, \
                    fntw, fntheight, \
                    color, \
                    op, \
                    8*fntglyphw); \
        } \
        if (bold) { \
          if (!calc) \
            bitblit_P(fntg+4, \
                      dx+1, dy0, \
                      fntw, \
                      fntheight, \
                      color, \
                      ESPVGAX2_OP_OR, \
                      8*fntglyphw); \
          dx++; \
        } \
        dx+=fntw+1; \
      } else { \
        dx+=2+hspace; \
      } \
    } \
  } \
  if (dx>maxx) \
    maxx=dx;

#define PRINTCODE(reader) \
  int dx0=dx; \
  int maxx=0; \
  int dlen=len; \
  uint8_t default_color=color; \
  int8_t *pstr=(int8_t*)str; \
  int8_t c; \
  while (0!=(c=reader(pstr++))) { \
    PRINT_LOOP_CODE(reader); \
    if (len>-1 && --dlen<=0) \
      break; \
  } \
  return PrintInfo(dx, dy0, maxx); 

ESPVGAX2::PrintInfo ESPVGAX2::print_P(ESPVGAX2_PROGMEM const char *str, int dx, 
  int dy0, uint8_t color, bool wrap, int len, int op, bool bold, 
  int dx0override, bool calc) {

  PRINTCODE(pgm_read_byte);
}
ESPVGAX2::PrintInfo ESPVGAX2::print(const char *str, int dx, int dy0, 
  uint8_t color, bool wrap, int len, int op, bool bold, int dx0override, 
  bool calc) {

  PRINTCODE(*);
}
