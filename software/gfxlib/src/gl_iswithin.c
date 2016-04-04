// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

BOOL_8 gl_iswithin(gl_point *p, gl_rect *r)
{
  return ( (p->x >= r->left) && (p->x <= r->right) && (p->y >= r->top) && (p->y <= r->bottom) );
}
