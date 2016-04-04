// Bitfire Graphics Library, Graphics Effects
// mtrojer@arrownordic.com
// $WCREV$
//! The Graphics Library effects part.
//! \page GFXFX gfxfx.h
//! \ingroup GFXLIBINC

#include "gfxlib.h"

#ifndef __GFXFX_H
#define __GFXFX_H

typedef struct
{
  UNS_8 b1;
  UNS_8 b2;
  UNS_8 b3;
  UNS_8 b4;
} glfx_plasmavars;

typedef struct
{
  float rz;
  float speed;
  INT_16 rx, ry;
  INT_16 sx, sy;
  UNS_8 color;
} glfx_star;

//! \defgroup gfxfx gfxfx

#ifdef __cplusplus

// TODO: Add comments (docs)
extern "C" void glfx_parallax(UNS_8 *pbuf, UNS_16 angle);
extern "C" BOOL_8 glfx_pixsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset, BOOL_8 dbl);
extern "C" BOOL_8 glfx_scrolltext(CHAR *text, gl_point *pos, gl_col *col, UNS_32 *offset, BOOL_8 dbl);			
extern "C" BOOL_8 glfx_charsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset, BOOL_8 dbl);

extern "C" void glfx_pattern1(UNS_32 *offset);
extern "C" void glfx_pattern2(UNS_32 *offset);
extern "C" void glfx_pattern3(UNS_32 *offset, const INT_8 *sintable, UNS_16 sinlen);
extern "C" void glfx_pattern4(UNS_32 *offset, const INT_8 *sintable, UNS_16 sinlen);

extern "C" void glfx_init_redfire_palette(gl_col *palette);
extern "C" void glfx_init_yellowfire_palette(gl_col *palette);
extern "C" void glfx_init_greenfire_palette(gl_col *palette);
extern "C" void glfx_init_radient_palette(gl_col *palette);


extern "C" void glfx_3dstar(glfx_star *stars, UNS_16 starnum);
extern "C" void glfx_fire(UNS_8 *pbuf, UNS_8 YSIZE);
extern "C" void glfx_fire2(UNS_8 *pbuf, UNS_8 YSIZE);
extern "C" void glfx_plasma(UNS_8 *pbuf, const INT_8 *costable, glfx_plasmavars *pvs);

extern "C" void glfx_mandel(INT_32 maxiter, UNS_8 co);
extern "C" void glfx_julia(INT_32 maxiter, UNS_8 co);

#else

void glfx_parallax(UNS_8 *pbuf, UNS_16 angle);
BOOL_8 glfx_pixsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset, BOOL_8 dbl);

//! Write a simple fullscreen cyclic pattern ver 1 to working buffer.
//! @param *offset pointer to offset counter that controls the pattern cycle.
//! Needs to be incremented by calling function.
//!\ingroup gfxfx
void glfx_pattern1(UNS_32 *offset);

//! Write a simple fullscreen cyclic pattern ver 2 to working buffer.
//! @param *offset pointer to offset counter that controls the pattern cycle.
//! Needs to be incremented by calling function.
//!\ingroup gfxfx
void glfx_pattern2(UNS_32 *offset);

//! Write a vertical fullscreen cyclic bars to working buffer.
//! @param *offset pointer to offset counter that controls the pattern cycle.
//! Needs to be incremented by calling function.
//! @param *sintable pointer to sintable that defines the bars
//! @param sinlen length of the sintable.
//!\ingroup gfxfx
void glfx_pattern3(UNS_32 *offset, const INT_8 *sintable, UNS_16 sinlen);

//! Write a horizontal fullscreen cyclic bars to working buffer.
//! @param *offset pointer to offset counter that controls the pattern cycle.
//! Needs to be incremented by calling function.
//! @param *sintable pointer to sintable that defines the bars
//! @param sinlen length of the sintable.
//!\ingroup gfxfx
void glfx_pattern4(UNS_32 *offset, const INT_8 *sintable, UNS_16 sinlen);

//! Generate 256 color palette, biased on red.
//! @param *palette pointer to palette buffer, 256 gl_cols big.
//!\ingroup gfxfx
void glfx_init_redfire_palette(gl_col *palette);

//! Generate 256 color palette, biased on yellow.
//! @param *palette pointer to palette buffer, 256 gl_cols big.
//!\ingroup gfxfx
void glfx_init_yellowfire_palette(gl_col *palette);

//! Generate 256 color palette, biased on green.
//! @param *palette pointer to palette buffer, 256 gl_cols big.
//!\ingroup gfxfx
void glfx_init_greenfire_palette(gl_col *palette);

//! Generate 256 color 'radient' palette, biased on red.
//! @param *palette pointer to palette buffer, 256 gl_cols big.
//!\ingroup gfxfx
void glfx_init_radient_palette(gl_col *palette);

//! Normal scrolltext in working buffer.
//! @param *text String to scroll
//! @param *pos y top scrolled line
//! @param *col Pointer to color(r,g) struct.
//! @param *offset internal counter, is updated, start at 0.
//! @return Returns True when scroll is done, otherwise 0.
//!\ingroup gfxfx
//BOOL_8 glfx_scrolltext(CHAR *text, gl_point *pos, gl_col *col, UNS_32 *offset);			
BOOL_8 glfx_scrolltext(CHAR *text, gl_point *pos, gl_col *col, UNS_32 *offset, BOOL_8 dbl);			

//! Scrolltext where the chars follows sinvalues. This takes up whole screen.
//! Writes to working buffer.
//! @param *text String to scroll
//! @param *col Pointer to color(r,g) struct.
//! @param *sintable should have XSIZE (40) elements and amplitude of YSIZE/2 (8).
//! @param *offset internal counter, is updated, start at 0.
//! @return Returns True when scroll is done, otherwise 0.
//!\ingroup gfxfx
//BOOL_8 glfx_charsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset);
BOOL_8 glfx_charsinscroll(CHAR *text, gl_col *col, const INT_8 *sintable, UNS_32 *offset, BOOL_8 dbl);

//! Animated 3D star field, uses floats.
//! @param *stars pointer to array go glfx_star structs (init to 0)
//! @param starnum number of stars
//!\ingroup gfxfx
void glfx_3dstar(glfx_star *stars, UNS_16 starnum);

//! Fire effect
//! You need to call gl_pblit (with a valid palette) after each glfx_fire() call
//! to update the working buffer.
//! @param *pbuf local screen buffer that consists of UNS_8 palette-colored indexes.
//! @param YSIZE number of XSIZE-ed lines in the pbuf buffer.
//!\ingroup gfxfx
void glfx_fire(UNS_8 *pbuf, UNS_8 YSIZE);

//! Fire effect with 'real' flames
//! You need to call gl_pblit (with a valid palette) after each glfx_fire() call
//! to update the working buffer.
//! @param *pbuf local screen buffer, that consists of UNS_8 palette-colored indexes.
//! @param YSIZE number of XSIZE-ed lines in the pbuf buffer.
//!\ingroup gfxfx
void glfx_fire2(UNS_8 *pbuf, UNS_8 YSIZE);

//! Plasma effect
//! You need to call gl_pblit (with a vaild palette) after each glfx_plasma() call
//! to update the working buffer.
//! @param *pbuf local screen buffer, that consists of UNS_8 palette-colored indexes.
//! @param *costable should have 256 elements.
//! @param *pvs struct plasmavars, will be overwritten, init all to 0.
//!\ingroup gfxfx
void glfx_plasma(UNS_8 *pbuf, const INT_8 *costable, glfx_plasmavars *pvs);

//! Draw a Mandelbrot fractal on the whole workbuf.
//! (Uses floats).
//! @param maxiter maximum numer of iterations in fractal loop.
//! @param co color offset
//!\ingroup gfxfx
void glfx_mandel(INT_32 maxiter, UNS_8 co);

//! Draw a Juila fractal on the whole workbuf.
//! (Uses floats).
//! @param maxiter maximum numer of iterations in fractal loop.
//! @param co color offset
//!\ingroup gfxfx
void glfx_julia(INT_32 maxiter, UNS_8 co);

#endif

#endif
