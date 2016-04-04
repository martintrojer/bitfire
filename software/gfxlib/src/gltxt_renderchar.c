// Bitfire Graphics Library, Render a character
// mtrojer@arrownordic.com
// $WCREV$
//

#include "gfxlib.h"
#include "gfxtxt.h"

const UNS_16 *myfont = 0;
UNS_8 myxsize = 0;
UNS_8 myysize = 0;

// TODO: fix hardcoded sintable size
#define SIN_SIZE GL_XSIZE*GL_XBOARD+16

void gltxt_setfont(const UNS_16 *font, UNS_8 xsize, UNS_8 ysize)
{
  myfont = font;
  myxsize = xsize;
  myysize = ysize;
}

void gltxt_getfontsize(UNS_8 *xsize, UNS_8 *ysize)
{
  *xsize = myxsize;
  *ysize = myysize;
}

void gltxt_renderchar(CHAR ch, gl_point *pos, gl_col *col)
{
  UNS_16 i,j;
  UNS_16 current_line;
  
  for (j=0;j<myysize;j++) {
    current_line = myfont[(myysize*ch)+j];
    for (i=0;i<myxsize;i++) {
      if (current_line & (UNS_16) 0x8000)
	gl_setpixelxy(pos->x+i, pos->y+j, col);
      current_line = current_line << 1;
    }
  }
}

void gltxt_sinrenderchar(CHAR ch, INT_16 x, INT_16 y, gl_col *col, const INT_8 *sintable, UNS_16 sinoffset)
{
  UNS_16 i,j;
  UNS_16 current_line;
  INT_16 offset;
  INT_32 o;

  for (j=0;j<myysize;j++) {
    current_line = myfont[(myysize*ch)+j];
    for (i=0;i<myxsize;i++) {
      if (current_line & (UNS_16) 0x8000) {
	if (x+myxsize<0)
	  offset = sintable[0];
	else
	  o = x+myxsize+i-sinoffset;
	  while (o<0) o += SIN_SIZE;
	  while (o>=SIN_SIZE) o -= SIN_SIZE;
	  offset = sintable[o];
	gl_setpixelxy(x+i, y+j+offset, col);
      }
      current_line = current_line << 1;
    }
  }
}

void gltxt_renderchar_dbl(CHAR ch, gl_point *pos, gl_col *col)
{
  UNS_16 i,j;
  UNS_16 current_line;
  
  for (j=0;j<myysize;j++) {
    current_line = myfont[(myysize*ch)+j];
    for (i=0;i<myxsize;i++) {
      if (current_line & (UNS_16) 0x8000) {
	gl_setpixelxy(2*(pos->x+i)+0, 2*(pos->y+j)+0, col);
	gl_setpixelxy(2*(pos->x+i)+1, 2*(pos->y+j)+0, col);
	gl_setpixelxy(2*(pos->x+i)+0, 2*(pos->y+j)+1, col);
	gl_setpixelxy(2*(pos->x+i)+1, 2*(pos->y+j)+1, col);
      }
      current_line = current_line << 1;
    }
  }
}

void gltxt_sinrenderchar_dbl(CHAR ch, INT_16 x, INT_16 y, gl_col *col, const INT_8 *sintable, UNS_16 sinoffset)
{
  UNS_16 i,j;
  UNS_16 current_line;
  INT_16 offset,offset2;
  INT_32 o,o2;

  for (j=0;j<myysize;j++) {
    current_line = myfont[(myysize*ch)+j];
    for (i=0;i<myxsize;i++) {
      if (current_line & (UNS_16) 0x8000) {
	if (x+myxsize<0)
	  offset = offset2 = sintable[0];
	else {
	  o = x+myxsize+i-sinoffset;
	  o2 = x+myxsize+i+1-sinoffset;
	  while (o<0) o += SIN_SIZE;
	  while (o>=SIN_SIZE) o -= SIN_SIZE;
	  while (o2<0) o2 += SIN_SIZE;
	  while (o2>=SIN_SIZE) o2 -= SIN_SIZE;
		
	  offset = sintable[o];
	  offset2 = sintable[o2];
	}
	gl_setpixelxy(2*(x+i)+0, 2*(y+j+offset)+0, col);
	gl_setpixelxy(2*(x+i)+1, 2*(y+j+offset2)+0, col);
	gl_setpixelxy(2*(x+i)+0, 2*(y+j+offset)+1, col);
	gl_setpixelxy(2*(x+i)+1, 2*(y+j+offset2)+1, col);
      }
      current_line = current_line << 1;
    }
  }
}
