// Bitfire Graphics Library, Mandelbrot Fractal
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_mandel(INT_32 maxiter, UNS_8 co)
{
  INT_32 _xmax = GL_XSIZE*GL_XBOARD;
  INT_32 _ymax = GL_YSIZE*GL_YBOARD;
  INT_32 sx=_xmax;
  INT_32 sy=_ymax;
  float xmin=-2;
  float xmax=1.25;
  float ymin=-1.25;
  float ymax=1.25;
  //int maxiter=96;
  
  float old_x;
  float fx,fy;
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
      fx=0;           
      fy=0;
      m=0;            
      do {
	old_x=fx;      
	fx=fx*fx-fy*fy+x;   
	fy=2*old_x*fy+y;  
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
