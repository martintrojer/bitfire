// Bitfire Graphics Library, Scroll texter manager
// mtrojer@arrownordic.com
// $WCREV$
//
// 'ÿ' in text string means command (will be replaced by ' ' in scrolled text).
// Will 'execute' next command in scroll_cmd list.
//

#include "gfxlib.h"
#include "gfxtxt.h"
#include "gfxfx.h"

// TODO: Fix hardcoded tables
static const INT_8 sintable[GL_XSIZE*GL_XBOARD+16];
//= {16, 17, 18, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 30, 31, 31, 31, 32, 31, 31, 31, 30, 30, 29, 28, 27, 26, 25, 24, 23, 21, 20, 19, 17, 16, 15, 14, 12, 11, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15, 16, 17, 18, 20, 21, 23, 24, 25, 26, 27, 28, 29, 30, 30, 31, 31, 31, 32, 31, 31, 31, 30, 30, 29, 28, 27, 26, 25, 24, 23, 21, 20, 19, 17, 16, 15, 14, 12, 11, 9, 8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0, 1, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 15};
static const INT_8 sintable_dbl[GL_XSIZE*GL_XBOARD+16];
//= {4, 4, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 11, 11, 11, 12, 11, 11, 11, 11, 11, 10, 10, 9, 9, 8, 8, 7, 6, 6, 5, 4, 4, 4, 3, 2, 2, 1, 0, 0, -1, -1, -2, -2, -3, -3, -3, -3, -3, -4, -3, -3, -3, -3, -3, -2, -2, -1, -1, 0, 0, 1, 2, 2, 3, 4, 4, 4, 5, 6, 6, 7, 8, 8, 9, 9, 10, 10, 11, 11, 11, 11, 11, 12, 11, 11, 11, 11, 11, 10, 10, 9, 9, 8, 8, 7, 6, 6, 5, 4, 4, 4, 3, 2, 2, 1, 0, 0, -1, -1, -2, -2, -3, -3, -3, -3, -3, -4, -3, -3, -3, -3, -3, -2, -2, -1, -1, 0, 0, 1, 2, 2, 3, 4};
static INT_32 offset=0; 

static UNS_8 type=0;
static UNS_8 speed=0;
static INT_16 p_ctr=0;
static UNS_16 last_cmd = 0;
static INT_8 cmd_ctr = 0;
static gl_col color = { 255, 255 };

BOOL_8 gltxt_scroller(const CHAR *text, const scroll_cmd *cmd_list)
{
  int i,x;
  BOOL_8 done=0;
  UNS_8 xsize, ysize;
  scroll_cmd sc;

  gltxt_getfontsize(&xsize,&ysize);

  // Find next cmd on screen
  if (p_ctr==0)
    {
      i=0;
      while(text[i]!=0) {
	if (type>2)
	  x = ((GL_XSIZE*GL_XBOARD)/2-offset)+(i++*xsize);
	else
	  x = ((GL_XSIZE*GL_XBOARD)-offset)+(i++*xsize);

	if (x>GL_XSIZE*GL_XBOARD || x<-xsize)
	  continue;
	
	if (i>last_cmd && text[i]=='ÿ') {
	  last_cmd = i;
	  sc = cmd_list[cmd_ctr++];
	  type = sc.type;
	  p_ctr = sc.pause;
	  speed = sc.speed;
	  color.r = sc.color.r;
	  color.g = sc.color.g;
	  break;
	}		
      }
    }

  if (p_ctr>0) {
    offset--;
    p_ctr--;
  }

  switch (type)
    {
    case 0:
      {
	gl_point p = {0,20};
	if (glfx_scrolltext(text,&p,&color,&offset,0))
	  done = 1;
      }
      break;
    case 1:
      {
	if (glfx_charsinscroll(text,&color,(const INT_8*)&sintable,&offset,0))
	  done = 1;
      }
      break;
    case 2:
      {		
	if (glfx_pixsinscroll(text,&color,(const INT_8*)&sintable,&offset,0))
	  done = 1;
      }
      break;
    case 3:
      {
	gl_point p = {0,8};
	if (glfx_scrolltext(text,&p,&color,&offset,1))
	  done = 1;
      }
      break;
    case 4:
      {		
	if (glfx_charsinscroll(text,&color,(const INT_8*)&sintable_dbl,&offset,1))
	  done = 1;
      }
      break;
    case 5:
      {		
	if (glfx_pixsinscroll(text,&color,(const INT_8*)&sintable_dbl,&offset,1))
	  done = 1;
      }
      break;
    default:
      break;
    }

  if (p_ctr==0)
    offset += speed;

  if (done) {
    offset = 0;
	cmd_ctr = 0;
	type = speed = 0;
	p_ctr = last_cmd = 0;
    color.r = 255;
    color.g = 255;
    return 1;
  }
  else
    return 0;
}
