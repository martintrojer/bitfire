// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

// TODO: Fix the XBOARD re-calculation, add palette setpixel()

#include "gfxlib.h"

void gl_pblit(UNS_8 *source, gl_rect *srect, gl_point *dpos, gl_col *palette)
{
  UNS_16 sx,sy;
  UNS_16 x=dpos->x;
  UNS_16 y=dpos->y;

  for (sy=srect->top;sy<=srect->bottom;sy++) {
    for (sx=srect->left;sx<=srect->right;sx++) {
      gl_setpixelxy(x,y,&palette[*(source+sx+sy*GL_XSIZE*GL_XBOARD)]);
      x++;
    }
    y++;
    x=dpos->x;
  }
}
