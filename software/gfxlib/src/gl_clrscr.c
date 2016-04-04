// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_clrscr(gl_col *col)
{
  UNS_16 x,y;

  for (x=0; x<GL_XSIZE*GL_XBOARD; x++)
    for (y=0; y<GL_YSIZE*GL_YBOARD; y++)
      gl_setpixelxy(x,y,col);
}
