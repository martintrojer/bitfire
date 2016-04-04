// Bitfire Graphics Library, Simple Scrolling Text
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"
#include "gfxtxt.h"

BOOL_8 glfx_scrolltext(CHAR *text, gl_point *pos, gl_col *col, UNS_32 *offset, BOOL_8 dbl)
{
  UNS_32 i=0;
  BOOL_8 dirty = 1;
  UNS_8 xsize, ysize;

  gltxt_getfontsize(&xsize,&ysize);
  
  while(text[i]!=0)
    {
      INT_32 x;
      gl_point p;
	  
      if (dbl)
	x = ((GL_XSIZE*GL_XBOARD)/2-*offset)+(i++*xsize);
      else
	x = ((GL_XSIZE*GL_XBOARD)-*offset)+(i++*xsize);
      
      if (x>(GL_XSIZE*GL_XBOARD) || x<-xsize)
	continue;
      
      p.x = x;
      p.y = pos->y;

      if (dbl)
	gltxt_renderchar_dbl(text[i-1],&p,col);
      else
	gltxt_renderchar(text[i-1],&p,col);

      dirty = 0;
    }
  
  *offset += 1;
  return dirty;
}
