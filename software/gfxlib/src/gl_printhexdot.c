// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_printhexdot(INT_32 val, gl_point *p, gl_col *c)
{
  UNS_8 i;

  for (i=0;i<32;i++) {
    if (val & 0x80000000)
      gl_setpixelxy(p->x+i,p->y,c);
    val = val<<1;	
  }
}
