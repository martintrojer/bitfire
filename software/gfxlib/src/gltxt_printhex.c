// Bitfire Graphics Library, Graphics Text
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxtxt.h"

#define FONT_SIZE 8

void gltxt_printhex(UNS_32 hexval, gl_col *col, BOOL_8 shortval)
{
  gl_point p = {0,0};
  UNS_32 font_value;
  CHAR char_value;
  UNS_16 i, loopmax;

  if (shortval) {
    loopmax=4;
    hexval = hexval << (4*4);
  }
  else
    loopmax=8;
  
  for (i=0;i<loopmax;i++)
    {
      font_value = (hexval & 0xF0000000) >> 28;

      if (font_value < 10) // convert to a character
	char_value = 0x30 + font_value;
      else
	char_value = 0x41 + (font_value-0xa);
      
      gltxt_renderchar(char_value, &p, col);
      if (p.x>=(GL_XSIZE*GL_XBOARD)-8) {
	p.x=0;
	p.y=8;
      }
      else
	p.x = p.x + FONT_SIZE;
      
      hexval = hexval << 4; // next character
    }
}
