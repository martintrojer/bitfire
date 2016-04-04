// Bitfire Graphics Library, Radient Palette
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"
#include "gfxfx.h"

void glfx_init_radient_palette(gl_col *palette)
{
  UNS_16 x;
  for (x=0;x<256;x++)
    palette[x].r = palette[x].g = 0;
  for (x=1; x<=32; x++) {
    palette[x].r     = 0;
    palette[x].g     = 0;
    palette[x+32].r  = x*2-1;
    palette[x+32].g  = 0;
    palette[x+64].r  = 63;
    palette[x+64].g  = x*2-1;
    palette[x+96].r  = 63; 
    palette[x+96].g  = 63; 
    palette[x+128].r = 63;
    palette[x+128].g = 63;
    palette[x+160].r = 63;
    palette[x+160].g = 63;
    palette[x+192].r = 63;
    palette[x+192].g = 63-(x*2-1);
    palette[x+224].r = 63-(x*2-1); 
    palette[x+224].g = 0; 
  }
}

/*	
	for (x=1; x<=32; x++) {
        setrgb(x,     0,          0,          x*2-1);			// Blue
        setrgb(x+32,  x*2-1,      0,          63);
        setrgb(x+64,  63,         x*2-1,      63);
        setrgb(x+96,  63,         63,         63); 
        setrgb(x+128, 63,         63,         63);
        setrgb(x+160, 63,         63,         63-(x*2-1));
        setrgb(x+192, 63,         63-(x*2-1), 0);
		setrgb(x+224, 63-(x*2-1), 0,          0);				// Red
    }
*/
