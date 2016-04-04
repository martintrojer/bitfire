// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_ellipse(gl_point *p, UNS_8 a, UNS_8 b, gl_col *c)
{
  INT_32 xc = p->x;
  INT_32 yc = p->y;
  INT_32 a2 = a*a;
  INT_32 b2 = b*b;

  INT_32 x, y, dec;

  for (x = 0, y = b, dec = 2*b2+a2*(1-2*b); b2*x <= a2*y; x++)
  {
    gl_setpixelxy(xc+x,yc+y,c);
    gl_setpixelxy(xc-x,yc+y,c);
    gl_setpixelxy(xc+x,yc-y,c);
    gl_setpixelxy(xc-x,yc-y,c);

    if ( dec >= 0 )
      dec += 4*a2*(1-(y--));
    dec += b2*(4*x+6);
  }

  for (x = a, y = 0, dec = 2*a2+b2*(1-2*a); a2*y <= b2*x; y++)
  {
    gl_setpixelxy(xc+x,yc+y,c);
    gl_setpixelxy(xc-x,yc+y,c);
    gl_setpixelxy(xc+x,yc-y,c);
    gl_setpixelxy(xc-x,yc-y,c);

    if ( dec >= 0 )
      dec += 4*b2*(1-(x--));
    dec += a2*(4*y+6);
  }
}
