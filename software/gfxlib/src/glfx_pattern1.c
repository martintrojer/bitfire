// Bitfire Graphics Library, pattern1
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_pattern1(UNS_32 *offset)
{
  UNS_16 x,y;
  
  for (x=0;x<GL_XSIZE*GL_XBOARD;x++)
    for (y=0;y<GL_YSIZE*GL_YBOARD;y++) {
      gl_point p = {x,y};
      gl_col c = {((*offset*3)+y), ((*offset*3)+y)};
      gl_setpixel(&p,&c);
    }
}
