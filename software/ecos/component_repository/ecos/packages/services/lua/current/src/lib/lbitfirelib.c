// LUA bitfire lib
// Martin Trojer (mtrojer@arrownordic.com)

#include <pkgconf/system.h>
#ifndef CYGPKG_HAL_SYNTH

#define lbitfirelib_c

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "gfxlib/gfxlib.h"
#include "gfxlib/gfxtxt.h"
#include "gfxlib/gfxfx.h"
#include "gfxlib/gfxvec.h"

#include "gfxlib/font_rom8x8.h"
#include "gfxlib/gfxvec_cube3d.h"
#include "gfxlib/gfxvec_sword3d.h"

//===============================================================================
// gfxlib
//===============================================================================
static int bitfire_init (lua_State *L)
{
	gl_init();
	return 0;
}

static int bitfire_flipbuf (lua_State *L)
{
	gl_flipbuf();
	return 0;
}

static int bitfire_setpixel (lua_State *L)
{
	int x = luaL_checkint(L, 1);
	int y = luaL_checkint(L, 2);
	gl_col c;
	c.r = luaL_checkint(L, 3);
	c.g = luaL_checkint(L, 4);
	
	gl_setpixelxy(x,y,&c);
	return 0;
}

static int bitfire_clrscr (lua_State *L)
{
	gl_col c={0,0};
	
	gl_clrscr(&c);
	return 0;
}
static int bitfire_fillrect (lua_State *L)
{
	gl_rect r;
	gl_col c;
	
	r.left = luaL_checkint(L, 1);
	r.top = luaL_checkint(L, 2);
	r.right = luaL_checkint(L, 3);
	r.bottom = luaL_checkint(L, 4);
	c.r = luaL_checkint(L, 5);
	c.g = luaL_checkint(L, 6);
	
	gl_fillrect(&r,&c);
	return 0;
}
static int bitfire_line (lua_State *L)
{
	gl_point p1;
	gl_point p2;
	gl_col c;
	
	p1.x = luaL_checkint(L, 1);
	p1.y = luaL_checkint(L, 2);
	p2.x = luaL_checkint(L, 3);
	p2.y = luaL_checkint(L, 4);
	c.r = luaL_checkint(L, 5);
	c.g = luaL_checkint(L, 6);
	
	gl_line(&p1,&p2,&c);
	return 0;
}
static int bitfire_circle (lua_State *L)
{
	gl_point p;
	int r;
	gl_col c;
	
	p.x = luaL_checkint(L, 1);
	p.y = luaL_checkint(L, 2);
	r = luaL_checkint(L, 3);
	c.r = luaL_checkint(L, 4);
	c.g = luaL_checkint(L, 5);
	
	gl_circle(&p,r,&c);
	return 0;
}
static int bitfire_ellipse (lua_State *L)
{
	gl_point p;
	int a,b;
	gl_col c;
	
	p.x = luaL_checkint(L, 1);
	p.y = luaL_checkint(L, 2);
	a = luaL_checkint(L, 3);
	b = luaL_checkint(L, 4);
	c.r = luaL_checkint(L, 5);
	c.g = luaL_checkint(L, 6);
	
	gl_ellipse(&p,a,b,&c);
	return 0;
}

//===============================================================================
// gfxtxt
//===============================================================================
static int bitfire_renderchar(lua_State *L)
{
	int ch;
	gl_point p;
	gl_col c;
	
	ch = luaL_checkint(L, 1);
	p.x = luaL_checkint(L, 2);
	p.y = luaL_checkint(L, 3);
	c.r = luaL_checkint(L, 4);
	c.g = luaL_checkint(L, 5);
	
	gltxt_setfont(font_rom8x8_bits,8,8);
	gltxt_renderchar(ch,&p,&c);
	return 0;
}
static int bitfire_printtext(lua_State *L)
{
	char *str;
	gl_point p;
	gl_col c;
	
	str = luaL_checkstring(L, 1);
	p.x = luaL_checkint(L, 2);
	p.y = luaL_checkint(L, 3);
	c.r = luaL_checkint(L, 4);
	c.g = luaL_checkint(L, 5);
	
	gltxt_setfont(font_rom8x8_bits,8,8);
	gltxt_printtext(str,&p,&c);
	return 0;
}
static int bitfire_printhex(lua_State *L)
{
	int val;
	gl_col c;
	
	val = luaL_checkint(L, 1);
	c.r = luaL_checkint(L, 2);
	c.g = luaL_checkint(L, 3);
	
	gltxt_setfont(font_rom8x8_bits,8,8);
	gltxt_printhex(val,&c,0);
	return 0;
}

//===============================================================================
// gfxfx
//===============================================================================
static int gfxfx_tables_ok = 0;

static INT_8 mysintable[GL_XSIZE*GL_XBOARD+16];
static INT_8 mycostable[256];
UNS_8 pbuf[GL_XSIZE*GL_XBOARD][GL_YSIZE*GL_YBOARD];
static gl_col palette[256];
static glfx_plasmavars pvs;
#define STARNUMBER 256
static glfx_star stars[STARNUMBER];

static void gfxfx_setup(void)
{
	int i;
	if (gfxfx_tables_ok)
		return;
	
	for(i=0;i<256;i++)
		mycostable[i] = (int)( 30 * cos(i * (3.14159/16) ) );
	
	for(i=0;i<GL_XSIZE*GL_XBOARD+16;i++)
		mysintable[i] = (GL_YSIZE*GL_YBOARD/2)-8+(int)( (GL_YSIZE*GL_YBOARD/2-4) * sin(i * (3.14159/(GL_XSIZE*GL_XBOARD/4)) ) );
	
	for (i=0;i<STARNUMBER;i++)
    	stars[i].rz = stars[i].sx = stars[i].sy = 0;

	for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
		*((UNS_8*)pbuf+i) = 0;
	
	pvs.b1 = pvs.b2 = pvs.b3 = pvs.b4 = 0;
	
	gfxfx_tables_ok=1;
}
static int bitfire_scrolltext(lua_State *L)
{
	char *text;
	gl_point p;
	gl_col c;
	int offset;
	int dbl;
	int status;
	
	text = luaL_checkstring(L, 1);
	p.x = luaL_checkint(L, 2);
	p.y = luaL_checkint(L, 3);
	c.r = luaL_checkint(L, 4);
	c.g = luaL_checkint(L, 5);
	offset = luaL_checkint(L, 6);
	dbl = luaL_checkint(L, 7);
	
	gltxt_setfont(font_rom8x8_bits,8,8);
	status = glfx_scrolltext(text,&p,&c,&offset,dbl);
	
	lua_pushnumber(L,status);
	lua_pushnumber(L,offset);
	return 2;
}
static int bitfire_charsinscroll(lua_State *L)
{
	char *text;
	gl_col c;
	int offset;
	int dbl;
	int status;
	
	gfxfx_setup();
	
	text = luaL_checkstring(L, 1);
	c.r = luaL_checkint(L, 4);
	c.g = luaL_checkint(L, 5);
	offset = luaL_checkint(L, 6);
	dbl = luaL_checkint(L, 7);
	
	gltxt_setfont(font_rom8x8_bits,8,8);
	status = glfx_charsinscroll(text,&c,mysintable,&offset,dbl);
	
	lua_pushnumber(L,status);
	lua_pushnumber(L,offset);
	return 2;
}
static int bitfire_pixsinscroll(lua_State *L)
{
	char *text;
	gl_col c;
	int offset;
	int dbl;
	int status;
	
	gfxfx_setup();
	
	text = luaL_checkstring(L, 1);
	c.r = luaL_checkint(L, 4);
	c.g = luaL_checkint(L, 5);
	offset = luaL_checkint(L, 6);
	dbl = luaL_checkint(L, 7);
	
	gltxt_setfont(font_rom8x8_bits,8,8);
	status = glfx_pixsinscroll(text,&c,mysintable,&offset,dbl);
	
	lua_pushnumber(L,status);
	lua_pushnumber(L,offset);
	return 2;
}
static int bitfire_pattern1(lua_State *L)
{
	int offset;
	
	offset = luaL_checkint(L, 1);
	glfx_pattern1(&offset);
	
	return 0;
}
static int bitfire_pattern2(lua_State *L)
{
	int offset;
	
	offset = luaL_checkint(L, 1);
	glfx_pattern2(&offset);
	
	return 0;
}
static int bitfire_pattern3(lua_State *L)
{
	int offset;
	
	gfxfx_setup();
	
	offset = luaL_checkint(L, 1);
	glfx_pattern3(&offset,mycostable,256);
	
	return 0;
}
static int bitfire_pattern4(lua_State *L)
{
	int offset;
	
	gfxfx_setup();
	
	offset = luaL_checkint(L, 1);
	glfx_pattern4(&offset,mycostable,256);
	
	return 0;
}
static int bitfire_3dstar(lua_State *L)
{
	int num;
	
	num = luaL_checkint(L, 1);
	if (num>256)
		num=256;
	
	glfx_3dstar(stars,num);
	
	return 0;
}
static int bitfire_fire(lua_State *L)
{
	glfx_init_redfire_palette(palette);
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_point dp = {0,1};
	
	glfx_fire((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
	return 0;
}
static int bitfire_fire2(lua_State *L)
{
	glfx_init_yellowfire_palette(palette);
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_point dp = {0,0};
	
	glfx_fire2((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
	return 0;
}
static int bitfire_plasma(lua_State *L)
{
	gfxfx_setup();

	glfx_init_radient_palette(palette);
	
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD-1,GL_YSIZE*GL_YBOARD-1};
	gl_point dp = {0,0};
	
	glfx_plasma((UNS_8*)pbuf, mycostable, &pvs);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
	return 0;
}
static int bitfire_mandel(lua_State *L)
{
	int it;
	
	it = luaL_checkint(L, 1);
	
	glfx_mandel(it,0);
	
	return 0;
}
static int bitfire_julia(lua_State *L)
{
	int it;
	
	it = luaL_checkint(L, 1);
	
	glfx_julia(it,0);
	
	return 0;
}

//===============================================================================
// gfxfx
//===============================================================================
static int bitfire_vector_load(lua_State *L)
{
	int set;
	
	set = luaL_checkint(L, 1);
	set = set % 2;
	
	if (set==0)
		InitGVectors(cube3d,1);
	if (set==1)
		InitGVectors(sword3d,1);
	return 0;
}
static int bitfire_vector_render(lua_State *L)
{
	int zoom;
	gl_col c;
	
	zoom = 	luaL_checkint(L, 1);
	c.r = luaL_checkint(L, 2);
	c.g = luaL_checkint(L, 3);

	UpdateGVectors(zoom,&c);
	return 0;
}

//===============================================================================
// bitfirelib
//===============================================================================

static const luaL_reg bitfirelib[] = {
  {"init",   bitfire_init},
  {"flipbuf", bitfire_flipbuf},
  {"setpixel", bitfire_setpixel},
  {"clrscr", bitfire_clrscr},
  {"fillrect", bitfire_fillrect},
  {"line", bitfire_line},
  {"circle", bitfire_circle},
  {"ellipse", bitfire_ellipse},
  {"renderchar",bitfire_renderchar},
  {"printtext",bitfire_printtext},
  {"printhex",bitfire_printhex},
  {"scrolltext",bitfire_scrolltext},
  {"charsinscroll",bitfire_charsinscroll},
  {"pixsinscroll",bitfire_pixsinscroll},
  {"pattern1",bitfire_pattern1},
  {"pattern2",bitfire_pattern2},
  {"pattern3",bitfire_pattern3},
  {"pattern4",bitfire_pattern4},
  {"starfield",bitfire_3dstar},
  {"fire",bitfire_fire},
  {"fire2",bitfire_fire2},
  {"plasma",bitfire_plasma},
  {"mandel",bitfire_mandel},
  {"julia",bitfire_julia},
  {"vector_load",bitfire_vector_load},
  {"vector_render",bitfire_vector_render},
  {NULL, NULL}
};

LUALIB_API int luaopen_bitfire (lua_State *L) {
  luaL_openlib(L, LUA_BITFIRELIBNAME, bitfirelib, 0);
  return 1;
}

#endif
