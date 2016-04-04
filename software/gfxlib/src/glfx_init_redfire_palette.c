// Bitfire Graphics Library, Redfire Palette
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_init_redfire_palette(gl_col *palette)
{
  UNS_16 x;
  for (x=0;x<256;x++)
    palette[x].r = palette[x].g = 0;
  for (x=1; x<=32; x++) {
    palette[x].r    = x*1-1;			//x*2-1
    palette[x].g    = 0;
    palette[x+32].r = 63/2;				// 63
    palette[x+32].g = x*1-1;			//x*2-1
    palette[x+64].r = 63/2;				// 63
    palette[x+64].g = 63/2;				// 63
    palette[x+96].r = 0;
    palette[x+96].g = 63/2;				// 63
  }
}
