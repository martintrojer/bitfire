// Emulate the Display on the Kit
// mtrojer@arrownordic.com
// $WCREV$
//
// Each "pixel" is a red/green pair; 40 of these pairs per line.

#ifndef __disp_emu_h
#define __disp_emu_h

#include <windows.h>
#include "gfxlib.h"

#define PIX_X 3
#define PIX_Y 3
#define X_PIX_SPACE 1
#define Y_PIX_SPACE 1

#define X_BOARD_SPACE 5
#define Y_BOARD_SPACE 5

class DispEmu
{
public:
	void sethdc(HDC ahdc);
	void updatescr(void);
private:
	HDC myHDC;
};

extern "C" void setpixel(int,int, gl_col *);

extern "C" void bsp_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col);
extern "C" void bsp_setpixel(gl_point *pos, gl_col *col);
extern "C" void bsp_init_gfx(void);
extern "C" void bsp_flipbuf(void);
extern "C" void bsp_getid(gl_point *idx);

#endif