// Bitfire Graphics Library, Sinus Scrolling Text
// mtrojer@arrownordic.com
// $WCREV$
//

#include "gfxlib.h"
#include "gfxfx.h"
#include "gfxtxt.h"

// TODO: fix hardcoded sintable size
#define SIN_SIZE GL_XSIZE*GL_XBOARD+16

static UNS_16 sin_ctr=0;

BOOL_8 glfx_charsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset, BOOL_8 dbl)
{
  UNS_32 i=0;
  BOOL_8 dirty = 1;
  UNS_8 xsize, ysize;
  INT_32 o;

  gltxt_getfontsize(&xsize,&ysize);
  
  while(text[i]!=0)
    {
      INT_32 x;
      gl_point p;
      
      if (dbl)
	x = ((GL_XSIZE*GL_XBOARD)/2-*offset)+(i++*xsize);
      else
	x = ((GL_XSIZE*GL_XBOARD)-*offset)+(i++*xsize);
	  
      if (x>GL_XSIZE*GL_XBOARD || x<-xsize)
	continue;

      p.x = x;
      p.y = 4;
      
	  if (x+xsize>0) {
	//		p.y += sintable[(x+(*offset/10))%(GL_XSIZE*GL_XBOARD)];
    o = x+xsize-sin_ctr;
	while (o<0) o += SIN_SIZE;
	while (o>=SIN_SIZE) o -= SIN_SIZE;
	p.y += sintable[o];
	  }
      else
	p.y += sintable[0];
      
      if (dbl)
	gltxt_renderchar_dbl(text[i-1],&p,col);
      else
	gltxt_renderchar(text[i-1],&p,col);

      dirty = 0;
    }
  
	sin_ctr++;
  *offset += 1;
  return dirty;
}
