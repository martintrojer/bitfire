// Rendering text via gfxtxt (gfxlib)
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "gfxlib.h"
#include "gfxtxt.h"
#include "font_rom8x8.h"

int main(void)
{
  UNS_32 inc=0,i=0;
  gl_point p = {0,0};
  
  gl_init();
  gl_flipbuf();
  gltxt_setfont(font_rom8x8_bits,8,8);

  //  gltxt_printtext("ARM",&p,&glcol_yellow);

  while(1) {
    gl_clrscr(&glcol_black);
    gltxt_printhex(gl_random(),&glcol_yellow,0);
    gl_flipbuf();
    for(i=0;i<1000000;)
      i++;
  }
}

