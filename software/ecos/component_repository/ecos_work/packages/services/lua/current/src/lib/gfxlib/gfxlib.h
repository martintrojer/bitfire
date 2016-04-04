// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$
//! The Graphics Library main functions and structs.
//! \page GFXLIB gfxlib.h
//! \ingroup GFXLIBINC

// TODO: Add a big font.
// TODO: Add a "demoreel" editor. Main purpose is to get the text messages right,
// doesn't have to show the effects. It would be ultra cool if it could import images! :)

#include "bitfire_types.h"

#ifndef __GFXLIB_H
#define __GFXLIB_H

#define GL_XSIZE 40
#define GL_YSIZE 16

#ifndef GL_XBOARD
#define GL_XBOARD 1
#endif
#ifndef GL_YBOARD
#define GL_YBOARD 1
#endif

typedef struct
{
  UNS_16 x;
  UNS_16 y;
} gl_point;

typedef struct
{
  UNS_16 left;
  UNS_16 top;
  UNS_16 right;
  UNS_16 bottom;
} gl_rect;

typedef struct
{
  UNS_8 r;
  UNS_8 g;
} gl_col;

//! \defgroup gfxlib gfxlib

#ifdef __cplusplus

extern "C" const gl_col glcol_black;
extern "C" const gl_col glcol_yellow;
extern "C" const gl_col glcol_red;
extern "C" const gl_col glcol_green;

// TODO: Add comments (docs)
extern "C" void gl_getid(gl_point *idx);

extern "C" void gl_init();
extern "C" void gl_flipbuf(void);
extern "C" void gl_setpixel(gl_point *pos, gl_col *col);
extern "C" void gl_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col);
extern "C" void gl_clrscr(gl_col *col);
extern "C" void gl_fillrect(gl_rect *rect, gl_col *col);
extern "C" void gl_cliprect(gl_rect *rect);
extern "C" BOOL_8 gl_iswithin(gl_point *p, gl_rect *r);
extern "C" void gl_pblit(UNS_8 *source, gl_rect *srect, gl_point *dpos, gl_col *palette);
extern "C" void gl_line(gl_point *p1, gl_point *p2, gl_col *c);
extern "C" void gl_circle(gl_point *p, UNS_8 r, gl_col *c);
extern "C" void gl_ellipse(gl_point *p, UNS_8 a, UNS_8 b, gl_col *c);

extern "C" void gl_printhexdot(INT_32 val, gl_point *p, gl_col *c);
extern "C" UNS_32 gl_random();

#else

void gl_getid(gl_point *idx);

//! Glocal black color constant
//\ingroup gfxlib
extern const gl_col glcol_black;

//! Glocal yellow color constant
//\ingroup gfxlib
extern const gl_col glcol_yellow;

//! Glocal red color constant
//\ingroup gfxlib
extern const gl_col glcol_red;

//! Glocal green color constant
//\ingroup gfxlib
extern const gl_col glcol_green;

//! Initialize the gfxlib (Will setup and use SPI0!)
//!\ingroup gfxlib
void gl_init();

//! To control double buffering. This function flips working and show buffer.
//! The buffer flip is done at next vertsync by the FPGA, so you don't
//! have to bother about waiting for sync before calling this function.
//! Must be called once before first set_pixel().
//! If you don't want to use double buffering you can ignore this function,
//! by default work buffer=show buffer=0
//! (Uses SPI0)
//! Example usage:
//! <pre>
//!   gl_init();   \n
//!   gl_flipbuf();           // Initialize dubble buffering \n
//!   .. \n
//!   gl_setpixel(&p,&c); \n
//!   gl_flipbuf();           // Drawing done -- flip buffer! \n
//!   .. 
//! </pre>
//!\ingroup gfxlib
void gl_flipbuf(void);

//! Set a pixel in current working buffer.
//! (Uses SPI0)
//! @param *pos Pointer to position(x,y) struct.
//! @param *col Pointer to color(r,g) struct.
//! Example usage:
//! <pre>
//!   gl_point p = {5,10};  \n
//!   gl_col c = {128,64};  \n
//!   gl_setpixel(&p,&c);   
//! </pre>
//!\ingroup gfxlib
void gl_setpixel(gl_point *pos, gl_col *col);

//! Set a pixel in current working buffer. Doesn't use the
//! gl_point struct. (Uses SPI0)
//! @param x x position.
//! @param y y position.
//! @param *col Pointer to color(r,g) struct.
//!\ingroup gfxlib
void gl_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col);

//! Clear the current work buffer.
//! (Uses SPI0)
//! @param *col Pointer to color(r,g) struct.
//! Example usage:
//! <pre>
//!  gl_clrscr(&glcol_black);
//! </pre>
//!\ingroup gfxlib
void gl_clrscr(gl_col *col);

//! Fill a gl_rect in work buffer with a given color.
//! (Uses SPI0)
//! @param *rect Pointer to rect(left,top,right,bottom) struct
//! @param *col Pointer to color(r,g) struct.
//! Example usage:
//! <pre>
//!  gl_rect r = {7,7,20,15}; \n
//!  gl_fillrect(&r,&glcol_yellow);
//! </pre>
//!\ingroup gfxlib
void gl_fillrect(gl_rect *rect, gl_col *col);

//! Modify a rect so it fits within the screen.
//! @param *rect Pointer to rect struct to be modified.
//!\ingroup gfxlib
void gl_cliprect(gl_rect *rect);

//! Check whether a point is within a rect.
//! @param *p Pointer to point struct
//! @param *r Pointer to rect struct.
//!\ingroup gfxlib
BOOL_8 gl_iswithin(gl_point *p, gl_rect *r);

//! Blit from a palette bitmap into the work buffer (with palette translations).
//! (Uses SPI0)
//! @param *source Pointer to 8 bit palette bitmap buffer. Note this buffer
//! has to have the same XSIZE as the work buffer!
//! @param *srect Source rect within the bitmap buffer to blit.
//! @param *dpos Start position in work buffer to start blitting to.
//! @param *palette Pointer to the palette.
//!\ingroup gfxlib
void gl_pblit(UNS_8 *source, gl_rect *srect, gl_point *dpos, gl_col *palette);

//! Draw a line (Bresenham) from p1 to p2 into work buffer.
//! (Uses gl_setpixel).
//! @param *p1 position 1 of gl_point struct.
//! @param *p2 position 2 of gl_point struct.
//! @param *c color of the line of gl_col struct.
//!\ingroup gfxlib
void gl_line(gl_point *p1, gl_point *p2, gl_col *c);

//! Draw a non-filled circle (Bresenahm) of radius r.
//! (Uses gl_setpixel).
//! @param *p center of the circle.
//! @param r radius of the circle.
//! @param *c color of the circle.
//!\ingroup gfxlib
void gl_circle(gl_point *p, UNS_8 r, gl_col *c);

//! Draw a non-filled ellipse (Bresenahm).
//! (Uses gl_setpixel).
//! @param *p center of the ellipse.
//! @param a x radius.
//! @param b y radius.
//! @param *c color of the ellipse
//!\ingroup gfxlib
void gl_ellipse(gl_point *p, UNS_8 a, UNS_8 b, gl_col *c);

//! Print a 32bit interger as 32 dots on the LED matrix.
//! This is useful for debug purposes.
//! (Uses gl_setpixel).
//!\ingroup gfxlib
void gl_printhexdot(INT_32 val, gl_point *p, gl_col *c);

//! Generate a random number (uses the c runtime lib).
//!\ingroup gfxlib
UNS_32 gl_random();

#endif

#endif
