// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_circle(gl_point *p, UNS_8 r, gl_col *c)
{
  INT_32 xc = p->x;
  INT_32 yc = p->y;
  INT_32 x,y,dec;

  for (x = 0, y = r, dec = 3-2*r; x <= y; x++)
  {
    gl_setpixelxy(xc+x,yc+y,c);
    gl_setpixelxy(xc+x,yc-y,c);
    gl_setpixelxy(xc-x,yc+y,c);
    gl_setpixelxy(xc-x,yc-y,c);
    gl_setpixelxy(xc+y,yc+x,c);
    gl_setpixelxy(xc+y,yc-x,c);
    gl_setpixelxy(xc-y,yc+x,c);
    gl_setpixelxy(xc-y,yc-x,c);

    if ( dec >= 0 )
      dec += -4*(y--)+4;
    dec += 4*x+6;
  }
}
