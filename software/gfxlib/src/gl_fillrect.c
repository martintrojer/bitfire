// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_fillrect(gl_rect *rect, gl_col *col)
{
  UNS_16 x,y;
  
  for (x=rect->left; x<rect->right; x++)
    for (y=rect->top; y<rect->bottom; y++)
      gl_setpixelxy(x,y,col);
}
