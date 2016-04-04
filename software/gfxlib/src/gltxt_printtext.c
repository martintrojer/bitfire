// Bitfire Graphics Library, Print text
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxtxt.h"

#define FONT_SIZE 8

void gltxt_printtext(CHAR *str, gl_point *pos, gl_col *col)
{
  gl_point p = *pos;
  while (*str != 0) {
    gltxt_renderchar(*str++,&p,col);
    p.x = p.x + FONT_SIZE;
  }
}

