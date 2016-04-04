// Bitfire Graphics Library, Graphics Text
// mtrojer@arrownordic.com
// $WCREV$
//! The Graphics Library text part.
//! \page GFXTXT gfxtxt.h
//! \ingroup GFXLIBINC

#include "gfxlib.h"

#ifndef __GFXTXT_H
#define __GFXTXT_H

typedef struct 
{
	gl_col color;
	UNS_16 pause;
	UNS_8 type;
	UNS_8 speed;
} scroll_cmd;

//! \defgroup gfxtxt gfxtxt

#ifdef __cplusplus

extern "C" const UNS_16 *myfont;
extern "C" const UNS_8 myxsize;
extern "C" const UNS_8 myysize;

// TODO: ADD comment
extern "C" BOOL_8 gltxt_scroller(const CHAR *text, const scroll_cmd *cmd_list);
extern "C" void gltxt_sinrenderchar(CHAR ch, INT_16 x, INT_16 y, gl_col *col, const INT_8 *sintable,UNS_16 sinoffset);
extern "C" void gltxt_getfontsize(UNS_8 *xsize, UNS_8 *ysize);
extern "C" void gltxt_renderchar_dbl(CHAR ch, gl_point *pos, gl_col *col);
extern "C" void gltxt_sinrenderchar_dbl(CHAR ch, INT_16 x, INT_16 y, gl_col *col, const INT_8 *sintable,UNS_16 sinoffset);

extern "C" void gltxt_setfont(const UNS_16 *font, UNS_8 xsize, UNS_8 ysize);
extern "C" void gltxt_renderchar(CHAR ch, gl_point *pos, gl_col *col);
extern "C" void gltxt_printtext(CHAR *str, gl_point *pos, gl_col *col);
extern "C" void gltxt_printhex(UNS_32 hexval, gl_col *col, BOOL_8 shortval);

#else

extern const UNS_16 *myfont;

BOOL_8 gltxt_scroller(const CHAR *text, const scroll_cmd *cmd_list);
void gltxt_sinrenderchar(CHAR ch, INT_16 x, INT_16 y, gl_col *col, const INT_8 *sintable,UNS_16 sinoffset);
void gltxt_getfontsize(UNS_8 *xsize, UNS_8 *ysize);
void gltxt_sinrenderchar_dbl(CHAR ch, INT_16 x, INT_16 y, gl_col *col, const INT_8 *sintable,UNS_16 sinoffset);
void gltxt_renderchar_dbl(CHAR ch, gl_point *pos, gl_col *col);


//! Set the font to use in upcoming text functions.
//! @param *font Pointer to font.
//!\ingroup gfxtxt
//void gltxt_setfont(const UNS_16 *font);
void gltxt_setfont(const UNS_16 *font, UNS_8 xsize, UNS_8 ysize);

//! Render a character to the LCD screen.
//! (uses gl_setpixel())
//! @param ch Charartcher to print.
//! @param *pos Pointer to position(x,y) struct.
//! @param *col Pointer to color(r,g) struct.
//!\ingroup gfxtxt
void gltxt_renderchar(CHAR ch, gl_point *pos, gl_col *col);

//! Print a string to the LCD screen. No automatic line-feed is done.
//! (uses gltxt_renderchar())
//! @param *str Pointer to string to render.
//! @param *pos Pointer to position(x,y) struct.
//! @param *col Pointer to color(r,g) struct.
//! <pre>
//!   gltxt_setfont(font_rom8x8_bits);                   \n
//!   gl_point p = {3,3};                                \n
//!   gltxt_printtext("Hello!",&p,&glcol_green);         
//! </pre>
//!\ingroup gfxtxt
void gltxt_printtext(CHAR *str, gl_point *pos, gl_col *col);

//! Print a hexadecimal value to the LED screen.
//! (uses gltxt_renderchar())
//! The value can either be 32 or 16 of length.
//! @param hexval Value to print
//! @param *col Pointer to color(r,g) struct.
//! @param shortval control to write 16 or 32 bit value.
//!\ingroup gfxtxt
void gltxt_printhex(UNS_32 hexval, gl_col *col, BOOL_8 shortval);

#endif

#endif
