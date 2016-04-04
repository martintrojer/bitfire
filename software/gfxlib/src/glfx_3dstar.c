// Bitfire Graphics Library, 3D Star
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

int DEPTH = 8;

void glfx_3dstar(glfx_star *stars, UNS_16 starnum)
{
  int i;
  gl_col c, black={0,0};

  for(i=0;i<starnum;i++)
    {
      gl_setpixelxy(stars[i].sx,stars[i].sy,&black);
      
      if (stars[i].rz <= 0) {	
	// Init new star
	stars[i].rx = ((gl_random()%(GL_XSIZE*GL_XBOARD)) - (GL_XSIZE*GL_XBOARD) / 2) * DEPTH / 2;
	stars[i].ry = ((gl_random()%(GL_YSIZE*GL_YBOARD)) - (GL_YSIZE*GL_YBOARD) / 2) * DEPTH / 2;
	stars[i].rz = (float)DEPTH;
	stars[i].speed = (4+(gl_random()%36)) / 100.0f;
      }

      stars[i].sx = (INT_16) (stars[i].rx / stars[i].rz + (GL_XSIZE*GL_XBOARD) / 2);
      stars[i].sy = (INT_16) (stars[i].ry / stars[i].rz + (GL_YSIZE*GL_YBOARD) / 2);
      stars[i].color = (UNS_8) ((stars[i].rz * 16 - 1));
      stars[i].rz -= stars[i].speed;
      
      c.r = c.g = 128 - stars[i].color;
      if (stars[i].sx>=0 && stars[i].sy>=0)
	gl_setpixelxy(stars[i].sx,stars[i].sy,&c);
    }
}
