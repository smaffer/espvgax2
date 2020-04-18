//file included by ESPVGAX2.cpp

/*
 * copy 32bits at a time. src and dst regions are aligned to 32 pixels. FAST
 */
#define BLIT32(op, reader) \
  const int srcw32=srcw/8; \
  const int srcwstride32=srcwstride/8; \
  const int dx32=dx/8; \
  uint32_t *src32=(uint32_t*)src; \
  volatile uint32_t *dst32=(volatile uint32_t*)&fbw[dy][dx32]; \
  for (int y=0; y!=srch; y++) { \
    if (!isYOutside(dy+y)) { \
      for (int x32=0; x32!=srcw32; x32++) { \
        if (isXOutside32(dx32+x32)) \
          continue; \
        dst32[x32] op reader(src32+x32); \
      } \
    } \
    src32+=srcwstride32; \
    dst32+=ESPVGAX2_WWIDTH; \
  }

/*
 * blit unaligned using 32bit read or 8bit read. slow version 
 */
#define BLITUNALIGNED(checkmask, exec, reader32, reader8) \
  const int srcw8=srcwstride/2; \
  for (int y=0; y!=srch; y++) { \
    if (!isYOutside(dy+y)) { \
      int x=0; \
      while (x<srcw) { \
        if (!isXOutside(dx+x)) { \
          if (x % 8) { \
            uint8_t p = reader8(src + y*srcw8 + (x/2)); \
            checkmask { exec(dx+x, dy+y, p); } \
            x++; \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
          } else { \
            uint32_t p = reader32(src + y*srcw8 + (x/2)); \
            /* unrolled loop for 8 pixels packed in 32bit */ \
            checkmask { exec(dx+x, dy+y, p); } \
            x++; \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              p>>=4; \
              checkmask { exec(dx+x, dy+y, p); } \
              x++; \
            } \
          } \
        } else \
          x++; \
      } \
    } \
  } 
#if 0 //NOT WORKING MACRO FROM ESPVGAX lib
/*
 * write 32bits at a time reading from unaligned src. src width can be aligned
 * to 32,16 or 8 pixels.
 * dst (dx,dy) can be unaligned too. in this case, each write generate a carry
 * that will be writen from the next 32bits write.
 */
#define BLITUNALIGNED(loop_code, final_code, reader32, reader16, reader8) \
  volatile uint32_t *dst=(volatile uint32_t*)&fbw[dy][dx/8]; \
  uint32_t dshift=dx % 8; \
  uint8_t pre=0; \
  if (dx<0 && dx%8) { \
    dst--; \
    pre=8; \
  } \
  while (srch>0) { \
    if (!isYOutside(dy)) { \
      uint32_t sx=0; \
      int sw=srcw; \
      int ldx=dx; \
      volatile uint32_t *d=dst;   \
      uint32_t carry=0, cmask=0; \
      uint32_t s=0; \
      while (sw>0) { \
        /* load from src */ \
        uint8_t *psrc=src+sx/2; \
        if (sw>4) { \
          s=reader32(psrc); \
        } else if (sw>2) { \
          s=reader16(psrc); \
        } else { \
          s=reader8(psrc); \
        } \
        /* write s to dst 32bit at a time */ \
        uint32_t dv=*d; \
        uint32_t dmask=0xffffffff; \
        if (sw<8) \
          dmask<<=4*(8-sw); \
        dmask<<=4*dshift; \
        if (!isXOutside(ldx)) { \
          loop_code \
          /* write back */ \
          *d=dv; \
        } \
        if (dshift && sw+pre+dshift>8) { \
          /* save carried bits not writen inside the current 32bit word */ \
          carry=s>>(4*(8-dshift)); \
          cmask=0xffffffff; \
          if (0 && sw<8) \
            cmask>>=4*(8-sw); \
          /*cmask<<=4*(dshift+(sw<8 ? sw : 0));*/ \
          /*carry=cmask;*/ \
        } else { \
          carry=0; \
          cmask=0; \
        } \
        sw-=8; \
        sx+=8; \
        ldx+=8; \
        d++; \
      } \
      if (cmask && 0) { /*cmask || srcw%8) {*/  \
        /* write carried bits, remaining of the last 32bit write of this line */ \
        if (!isXOutside(ldx) && (uint8_t*)d<fbb+ESPVGAX2_FBBSIZE) { \
          uint32_t dv=*d; \
          /*last s must be shifted to left*/ \
          final_code \
          *d=dv; \
        } \
      } \
    } \
    src+=srcwstride/2; \
    dst+=ESPVGAX2_WWIDTH; \
    srch--; \
    dy++; \
  } 
#endif
/*
 * implementation of BLIT with memory reading functions parametrized. these
 * functions will be pgm_read_* functions in the case of reading data from 
 * FLASH storage, and will be RAM reading deferencing operator (*) in case of
 * reading data from RAM.
 */
#define BLITMETHOD(checkmask, reader32, reader16, reader8, skip32) \
  if (dx>=ESPVGAX2_WIDTH || dy>=ESPVGAX2_HEIGHT) \
    return; \
  if (srcwstride==0) \
    srcwstride=srcw+(srcw % 8 ? 8-(srcw%8) : 0); \
  if (dx%8==0 && srcw%8==0 && !skip32) { \
    /* dx and srcw are aligned to 32bits. move dword instead of bits */ \
    if (op==ESPVGAX2_OP_OR) { \
      BLIT32(|=, reader32); \
    } else if (op==ESPVGAX2_OP_XOR) { \
      BLIT32(^=, reader32); \
    } else { \
      BLIT32(=, reader32); \
    } \
  } else { \
    if (op==ESPVGAX2_OP_OR) { \
      BLITUNALIGNED(checkmask, orpixel, reader32, reader8); \
    } else if (op==ESPVGAX2_OP_XOR) { \
      BLITUNALIGNED(checkmask, xorpixel, reader32, reader8); \
    } else { /*OP_SET*/ \
      BLITUNALIGNED(checkmask, setpixel, reader32, reader8); \
    } \
  } 
#if 0
    if (op==ESPVGAX2_OP_OR) { \
      BLITUNALIGNED( \
        { dv|=(s<<(4*dshift))|carry; }, \
        { dv|=carry; },  \
        reader32, reader16, reader8); \
    } else if (op==ESPVGAX2_OP_XOR) { \
      BLITUNALIGNED( \
        { dv^=(s<<(4*dshift))|carry; }, \
        { dv^=carry; },  \
        reader32, reader16, reader8); \
    } else { /*OP_SET*/ \
      BLITUNALIGNED( \
        { dv&=~(dmask|cmask); dv|=(s<<(4*dshift))|carry; }, \
        { dv&=~cmask; dv|=carry; },  \
        reader32, reader16, reader8); \
    } \
  }
#endif

/*
 * bitblit implementation as macro. reader8 can be pgm_read_byte or simple (*)
 */
#define BITBLITMETHOD_loop(exec, reader8) \
  const int srcw8=srcwstride/8; \
  color &= 0xf; \
  for (int y=0; y!=srch; y++) { \
    if (!isYOutside(dy+y)) { \
      int x=0; \
      while (x<srcw) { \
        if (!isXOutside(dx+x)) { \
          if (x % 8) { \
            uint8_t shift = 7-(x % 8); \
            uint8_t p = reader8(src + y*srcw8 + (x/8)); \
            if (p & (1<<shift)) \
              exec(dx+x, dy+y, color); \
            x++; \
          } else { \
            uint8_t p = reader8(src + y*srcw8 + (x/8)); \
            /* unrolled loop for 8 bits */ \
            if (p & (1<<7)) \
              exec(dx+x, dy+y, color); \
            x++; \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<6)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<5)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<4)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<3)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<2)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<1)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
            if (x<srcw && (dx+x)<ESPVGAX2_WIDTH) { \
              if (p & (1<<0)) \
                exec(dx+x, dy+y, color); \
              x++; \
            } \
          } \
        } else \
          x++; \
      } \
    } \
  } 
#define BITBLITMETHOD(reader8) \
  if (op==ESPVGAX2_OP_OR) { \
    BITBLITMETHOD_loop(orpixel, reader8); \
  } else if (op==ESPVGAX2_OP_XOR) { \
    BITBLITMETHOD_loop(xorpixel, reader8); \
  } else { /*OP_SET*/ \
    BITBLITMETHOD_loop(setpixel, reader8); \
  } \

void ESPVGAX2::blit_P(ESPVGAX2_PROGMEM uint8_t *src, int dx, int dy, int srcw, 
  int srch, int op, int srcwstride) {
  BLITMETHOD(if(true), pgm_read_dword, pgm_read_word, pgm_read_byte, false);
}
void ESPVGAX2::blit(uint8_t *src, int dx, int dy, int srcw, int srch, int op, 
  int srcwstride) {
  BLITMETHOD(if(true), *(uint32_t*), *(uint16_t*), *(uint8_t*), false);
}
void ESPVGAX2::blitwmask_P(ESPVGAX2_PROGMEM uint8_t *src, int dx, int dy, int srcw, 
  int srch, uint8_t mask, int op, int srcwstride) {
  BLITMETHOD(if((p&0xf) != mask), pgm_read_dword, pgm_read_word, pgm_read_byte, true);
}
void ESPVGAX2::blitwmask(uint8_t *src, int dx, int dy, int srcw, int srch, 
  uint8_t mask, int op, int srcwstride) {
  BLITMETHOD(if((p&0xf) != mask), *(uint32_t*), *(uint16_t*), *(uint8_t*), true);
}
void ESPVGAX2::bitblit_P(ESPVGAX2_PROGMEM uint8_t *src, int dx, int dy, 
  int srcw, int srch, uint8_t color, int op, int srcwstride) {
  BITBLITMETHOD(pgm_read_byte);
}
void ESPVGAX2::bitblit(uint8_t *src, int dx, int dy, int srcw, int srch, 
  uint8_t color, int op, int srcwstride) {
  BITBLITMETHOD(*(uint8_t*));
}
