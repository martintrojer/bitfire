// Bitfire Graphics Library, Pattern 4
// mtrojer@arrownordic.com
// $WCREV$

#include <math.h>
#include "gfxlib.h"
#include "gfxfx.h"

void glfx_pattern4(UNS_32 *offset, const INT_8 *sintable, UNS_16 sinlen)
{
  UNS_16 x,y;

  for (x=0;x<GL_XSIZE*GL_XBOARD;x++)
    for (y=0;y<GL_YSIZE*GL_YBOARD;y++) {
      gl_point p = {x,y};
      gl_col c = {4*abs(sintable[(*offset+y)%sinlen]), 1*abs(sintable[(*offset+y)%sinlen])};
      gl_setpixel(&p,&c);
    }
}
