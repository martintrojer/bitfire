// Bitfire Graphics Library, Julia Fractal
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_julia(INT_32 maxiter, UNS_8 co)
{
  INT_32 _xmax = GL_XSIZE*GL_XBOARD;
  INT_32 _ymax = GL_YSIZE*GL_YBOARD;
  INT_32 sx=_xmax;
  INT_32 sy=_ymax;
  float xmin=-2;
  float xmax=1.25;
  float ymin=-1.25;
  float ymax=1.25;

  float old_x;
  float fx,fy;
  float ix=0.42f;
  float iy=0.23f;
  INT_32 m;

  float dx=(xmax-xmin)/sx;
  float dy=(ymax-ymin)/sy;

  INT_32 px;
  INT_32 py=0;
  float x;
  float y=ymin;

  gl_col c;

  while (py<sy) {
    px=0;
    x=xmin;
    py++;
    while (px<sx) {
      px++;
      fx=x;
      fy=y;
      m=0;
      do {
	old_x=fx;
	fx= fx*fx - fy*fy + ix;
	fy= 2*old_x*fy + iy;
	m++;
      } while (((fx*fx+fy*fy)<4) && (m<maxiter));
//      *(screen + px +(py*_xmax)) = (short) (m+coloroff);
	  c.r = m+co;
	  c.g = m+co;
	  gl_setpixelxy(px,py,&c);
      x+=dx;
    }
    y+=dy;
  }
}
