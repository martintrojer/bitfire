// Some effects
// mtrojer@arrownordic.com
// $WCREV$

#include <math.h>

#include "bitfire_types.h"
#include "gfxlib.h"
#include "gfxtxt.h"
#include "gfxfx.h"
#include "gfxvec.h"
#include "font_rom8x8.h"
#include "gfxvec_cube3d.h"
//#include "gfxvec_sword3d.h"

INT_8 mysintable[GL_XSIZE*GL_XBOARD+16];
INT_8 mycostable[256];

//#define STARNUMBER 1
//glfx_star stars[STARNUMBER];

UNS_8 pbuf[GL_XSIZE*GL_XBOARD][GL_YSIZE*GL_YBOARD];
gl_col palette[256];
glfx_plasmavars pvs;

int main(void)
{
  UNS_32 offset = 0;
  UNS_32 scrolloffset = 0;
  BOOL_8 dirty = 0;
  int i;
  
  gl_init();
  gl_flipbuf();

  for(i=0;i<256;i++)
    mycostable[i] = (int)( 30 * cos(i * (3.14159/16) ) );

  for(i=0;i<GL_XSIZE*GL_XBOARD+16;i++)
    mysintable[i] = (GL_YSIZE*GL_YBOARD/2)-8+(int)( (GL_YSIZE*GL_YBOARD/2-4) * sin(i * (3.14159/(GL_XSIZE*GL_XBOARD/4)) ) );

//  for (i=0;i<STARNUMBER;i++)
//    stars[i].rz = stars[i].sx = stars[i].sy = 0;

  for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
    *((UNS_8*)pbuf+i) = 0;

  while(1)
    {

#if 0
      {	// ************** Lines, Circles and Ellipses ********************
        gl_point p1 = {0,0};
        gl_point p2 = {10,8};
        gl_line(&p1,&p2,(gl_col*)&glcol_green);
        gl_circle(&p2,7,(gl_col*)&glcol_green);
        gl_ellipse(&p2,10,4,(gl_col*)&glcol_green);
      }
#endif

#if 0
      { // ************** SIMPLE PATTERNS ********************
        //glfx_pattern2(&offset);
        glfx_pattern4(&offset,mycostable,256);
	offset++;
      }
#endif

#if 0
      {	// ************** STARFIELD ********************
	gl_clrscr(&glcol_black);
	glfx_3dstar((glfx_star*)stars, STARNUMBER);
      }
#endif

#if 0
      {	// ************** Fractals ********************
	//glfx_mandel(96,0);
        glfx_julia(96,0);
      }
#endif

#if 0
      {	// ************** 3D Graphics ********************
	gl_clrscr((gl_col*)&glcol_black);
	
	if (!dirty) {
	  InitGVectors(cube3d,1);
	  //InitGVectors(sword3d,1);
	  dirty = 1;
	}
	UpdateGVectors(512/18,(gl_col*)&glcol_green);
	//UpdateGVectors(512/8,(gl_col*)&glcol_green);
      }
#endif

#if 0
      {	// *********************** FIRE *************************
	if (offset==0) {
	  glfx_init_redfire_palette(palette);
	  offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_point dp = {0,1};
	
	glfx_fire((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
#endif

#if 0
      {	// *********************** FIRE2 *************************
	if (offset==0) {
	  glfx_init_yellowfire_palette(palette);
	  offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_point dp = {0,0};

	glfx_fire2((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
#endif

#if 1
      {	// *********************** PLASMA *************************
	if (offset==0) {
	  glfx_init_radient_palette(palette);
	  pvs.b1 = pvs.b2 = pvs.b3 = pvs.b4 = 0;
	  offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD-1,GL_YSIZE*GL_YBOARD-1};
	gl_point dp = {0,0};
	
	glfx_plasma((UNS_8*)pbuf, mycostable, &pvs);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
#endif


#if 0
      {	// ************ SCROLLTEXT ******************
	gltxt_setfont(font_rom8x8_bits,8,8);
	gl_point p = {0,0};
	gl_col c = {0,255};

//	gl_clrscr(&glcol_black);
	if (glfx_scrolltext("AtmosFire 2oo7 -- l337 waRez",&p,&c,&scrolloffset,0))
	  scrolloffset = 0;
      }
#endif

#if 1
      {	// ************ SINSCROLL ******************
	gltxt_setfont(font_rom8x8_bits,8,8);
	gl_point p = {0,0};
	gl_col c = {0,255};
	
//	gl_clrscr(&glcol_black);
        if (glfx_charsinscroll("Bitfire Development Kit",&c,mysintable,&scrolloffset,0))
	  scrolloffset = 0;
      }
#endif

//  gl_setpixelxy(0,0,&glcol_green);
                             gl_flipbuf();
#if 1
      { // ******** DELAY ********
	int j;
	for (j=0;j<100000;)
	  j++;
      }
#endif

      
    }
}

