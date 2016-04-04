// SetPixel via gfxlib
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "gfxlib.h"

int main(void)
{
  UNS_16 x,y,inc=0;
  
  gl_init();

  for (x=0;x<GL_XSIZE;x++) {
    inc++;
    for (y=0;y<GL_YSIZE;y++) {
      gl_point p={x,y};
      gl_col cr={x*6,0};
      gl_col cg={0,255-x*6};
      if ((y+inc)%2)
	gl_setpixel(&p,&cg);
      else
	gl_setpixel(&p,&cr);
    }
  }
  while(1)
  	;
}

