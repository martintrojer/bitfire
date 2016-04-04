// Bitfire Graphics Library, Greenfire Palette
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_init_greenfire_palette(gl_col *palette)
{
  UNS_16 x;
  for (x=0;x<256;x++)
    palette[x].r = palette[x].g = 0;
  for (x=1; x<=32; x++) {
    palette[x].r    = 0;
    palette[x].g    = x*2-1 ;
    palette[x+32].r = 0;
    palette[x+32].g = 63;
    palette[x+64].r = x*2-1;
    palette[x+64].g = 63;
    palette[x+96].r = 63;
    palette[x+96].g = 63;
  }
}
