// Bitfire Graphics Library, Plasma
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

// TODO: Add palette setpixel()

void glfx_plasma(UNS_8 *pbuf, const INT_8 *costable, glfx_plasmavars *pvs)
{
  UNS_8 a1=pvs->b1,a2=pvs->b1,a3,a4;
  UNS_8 *tscr = pbuf;
  UNS_16 x,y;
  
  for(y=0;y<GL_YSIZE*GL_YBOARD;y++)
    {
      a3 = pvs->b3;
      a4 = pvs->b4;
      
      for(x=0;x<GL_XSIZE*GL_XBOARD;x++)
	{
	  *tscr++ = costable[a1] + costable[a2] + costable[a3] + costable[a4];
	  // Higher values result in many slower plasmas
	  a3 += 1;
	  a4 += 2;
	}
      // same
      a1 += 2;
      a2 += 1;
    }
  // The higher these vars are incremented, the faster is the plasma.
  pvs->b1 += 1;
  pvs->b2 -= 2;
  pvs->b3 -= 1;
  pvs->b4 += 3;
}
