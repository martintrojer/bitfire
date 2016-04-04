// Bitfire Graphics Library, Yellowfire Palette
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_init_yellowfire_palette(gl_col *palette)
{
  UNS_16 x;
  for (x=0;x<256;x++)
    palette[x].r = palette[x].g = 0;
  for(x=0;x<64;x++) {
    palette[x].r     = 0;
    palette[x].g     = 0;
    palette[x+64].r  = x/2;				// x
    palette[x+64].g	 = x/4;				// x/2
    palette[x+128].r = 63/2;			// 63
    palette[x+128].g = 32/2+x/4;		//32+x/2
    palette[x+192].r = 63/2;
    palette[x+192].g = 63/2;			// 63
  }
}
