// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_line(gl_point *p1, gl_point *p2, gl_col *c)
{
  INT_32 x0 = p1->x;
  INT_32 y0 = p1->y;
  INT_32 x1 = p2->x;
  INT_32 y1 = p2->y;
	
  // starting point of line
  INT_32 x = x0, y = y0;

  // direction of line
  INT_32 dx = x1-x0, dy = y1-y0;

  // increment or decrement depending on direction of line
  INT_32 sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
  INT_32 sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));

  INT_32 ax, ay;
  INT_32 decx, decy;
  INT_32 max, var;
  
  // decision parameters for voxel selection
  if ( dx < 0 ) dx = -dx;
  if ( dy < 0 ) dy = -dy;
  ax = 2*dx;
  ay = 2*dy;

  // determine largest direction component, single-step related variable
  max = dx;
  var = 0;
  if ( dy > max ) { var = 1; }

  // traverse Bresenham line
  switch ( var )
  {
  case 0:  // single-step in x-direction
    for (decy=ay-dx; /**/; x += sx, decy += ay)
    {
      // process pixel
	  gl_setpixelxy(x,y,c);

      // take Bresenham step
      if ( x == x1 ) break;
      if ( decy >= 0 ) { decy -= ax; y += sy; }
    }
    break;
  case 1:  // single-step in y-direction
    for (decx=ax-dy; /**/; y += sy, decx += ax)
    {
      // process pixel
	  gl_setpixelxy(x,y,c);

      // take Bresenham step
      if ( y == y1 ) break;
      if ( decx >= 0 ) { decx -= ay; x += sx; }
    }
    break;
  }
}
