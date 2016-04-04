// Bitfire Graphics Library, Sinus Scrolling Text
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"
#include "gfxtxt.h"

static UNS_16 sin_ctr=0;

BOOL_8 glfx_pixsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset, BOOL_8 dbl)
{
  UNS_32 i=0;
  BOOL_8 dirty = 1;
  UNS_8 xsize, ysize;
  
  gltxt_getfontsize(&xsize,&ysize);

  while(text[i]!=0)
    {
      INT_32 x;

      if (dbl)
	x = ((GL_XSIZE*GL_XBOARD)/2-*offset)+(i++*xsize);
      else
	x = ((GL_XSIZE*GL_XBOARD)-*offset)+(i++*xsize);

      if (x>GL_XSIZE*GL_XBOARD || x<-xsize)
	continue;
            
      if (dbl)
	gltxt_sinrenderchar_dbl(text[i-1],x,4,col,sintable,sin_ctr);
      else
	gltxt_sinrenderchar(text[i-1],x,4,col,sintable,sin_ctr);
      dirty = 0;
    }
  
  sin_ctr++;
  *offset += 1;
  return dirty;
}
