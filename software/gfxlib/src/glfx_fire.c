// Bitfire Graphics Library, Fire
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

// TODO: paletta setpixel()

void glfx_fire(UNS_8 *pbuf, UNS_8 YSIZE)
{
  // fill the bottom lines with random values 
  UNS_8 *t,*val = pbuf+GL_XSIZE*GL_XBOARD*YSIZE;
  for (t=val-2*GL_XSIZE*GL_XBOARD;t<val;t++)
    *t = (gl_random()%2)==0 ? 0 : 128;
  
  val = pbuf+GL_XSIZE*GL_XBOARD*YSIZE-GL_XSIZE*GL_XBOARD;
  // calculate pixel values
  for( t=pbuf+GL_XSIZE*GL_XBOARD; t<val; t++ )
    {
      *(t-GL_XSIZE*GL_XBOARD) = ( *t + *(t+1) + *(t-1) + *(t+GL_XSIZE*GL_XBOARD) + *(t+GL_XSIZE*GL_XBOARD+1) + *(t+GL_XSIZE*GL_XBOARD-1) ) / 6; 
      // The fire needs to fade at the top
      if( *(t-GL_XSIZE*GL_XBOARD) > 0 )
	*(t-GL_XSIZE*GL_XBOARD) = *(t-GL_XSIZE*GL_XBOARD) - 1;
    }
}
