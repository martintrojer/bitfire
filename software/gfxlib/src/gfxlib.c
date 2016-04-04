// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

extern void bsp_init_gfx(void);
extern void bsp_flipbuf(void);
extern void bsp_setpixel(gl_point *pos, gl_col *col);
extern void bsp_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col);
extern void bsp_getid(gl_point *idx);

const gl_col glcol_black = {0,0};
const gl_col glcol_yellow = {255,255};
const gl_col glcol_red = {255,0};
const gl_col glcol_green = {0,255};

void gl_init()
{
  bsp_init_gfx();
}

void gl_flipbuf(void)
{
  bsp_flipbuf();
}

void gl_setpixel(gl_point *pos, gl_col *col)
{
/*
	gl_point idx;
	gl_rect r;

	gl_getid(&idx);
	r.left = idx.x*GL_XSIZE;
	r.top = idx.y*GL_YSIZE;
	r.right = r.left + GL_XSIZE;
	r.bottom = r.top + GL_YSIZE;

	if (gl_iswithin(pos,&r))
		pbuf[((pos->x)%GL_XSIZE)+((pos->y*GL_XSIZE)%GL_YSIZE)] = pcol;
*/	
	bsp_setpixel(pos,col);
}

void gl_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col)
{
  bsp_setpixelxy(x,y,col);
}

void gl_getid(gl_point *idx)
{
	bsp_getid(idx);
}
