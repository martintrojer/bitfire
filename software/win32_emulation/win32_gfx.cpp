// Handle graphics (setpixel) with the win32 CPU
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"

typedef struct
{
  UNS_16 x;
  UNS_16 y;
} gl_point;

typedef struct
{
  UNS_8 r;
  UNS_8 g;
} gl_col;

extern void setpixel(int,int, gl_col*);

void bsp_init_gfx(void)
{
}

void bsp_flipbuf(void)
{
}

void bsp_setpixel(gl_point *pos, gl_col *col)
{
  setpixel(pos->x,pos->y,col);
}

void bsp_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col)
{
  setpixel(x,y,col);
}
