// Bitfire Graphics Library, Fire2
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

// TODO: paletta setpixel()

void glfx_fire2(UNS_8 *pbuf, UNS_8 YSIZE)
{
  INT_16 offset=-1;
  UNS_16 decay=430;
  UNS_16 buildup=5;
  
  UNS_16 x,y;
  INT_32 temp;
  UNS_8 *destline, *line, *lineup, *linedown;
  
  for (x=0;x<GL_XSIZE*GL_XBOARD;x++)
    *(pbuf+x+(YSIZE-1)*GL_XSIZE*GL_XBOARD) = 255;

  for(y=1-offset;y<YSIZE-1-offset;y++)
    {
      destline=	(pbuf+((y+offset) *GL_XSIZE*GL_XBOARD));
      linedown=	(pbuf+((y-1)	  *GL_XSIZE*GL_XBOARD));
      line=		(pbuf+((y  )	  *GL_XSIZE*GL_XBOARD));
      lineup=		(pbuf+((y+1)	  *GL_XSIZE*GL_XBOARD));
      for(x=1;x<GL_XSIZE*GL_XBOARD-1;x++)
	{
	  temp=*(linedown+x)+*(line+x-1)+*(line+x)*2+*(line+x+1)+*(lineup+x)-gl_random()%decay;
	  if(temp<0) 
	    temp=0;
	  else if(temp!=0) 
	    temp/=buildup;
	  if(temp>255) 
	    temp=255;
	  *(destline+x)=temp;
	}
    }
}
