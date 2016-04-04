// Some effects
// mtrojer@arrownordic.com
// $WCREV$

#include <math.h>

#include "bitfire_types.h"
#include "gfxlib.h"
#include "gfxtxt.h"
#include "gfxfx.h"
#include "font_rom8x8.h"
#include "gfxvec.h"
#include "gfxvec_cube3d.h"
#include "gfxvec_sword3d.h"

INT_8 sintable[GL_XSIZE*GL_XBOARD+16];
INT_8 costable[256];

#define STARNUMBER 64
glfx_star stars[STARNUMBER];

#define MY_YSIZE GL_YSIZE+5

UNS_8 pbuf[GL_XSIZE][MY_YSIZE];
gl_col palette[256];
glfx_plasmavars pvs;

BOOL_8 dirty = 0;

static void generate_effect(UNS_32 effect, UNS_32 *offset)
{
  switch(effect)
    {
    case 0:
      glfx_pattern3(offset,costable,256);
      dirty = 0;
      break;
    case 1:
      gl_clrscr((gl_col*)&glcol_black);
	if (!dirty) {
	  InitGVectors(cube3d,1);
	  dirty = 1;
	}
	UpdateGVectors(512/18,(gl_col*)&glcol_red);
      break;
    case 2:
      glfx_pattern4(offset,costable,256);
      dirty = 0;
      break;
    case 3:
      {
	if (*offset==0) {
	  int i;
	  for (i=0;i<STARNUMBER;i++)
	    stars[i].rz = stars[i].sx = stars[i].sy = 0;
	  *offset=1;
	}
	gl_clrscr((gl_col*)&glcol_black);
	glfx_3dstar((glfx_star*)stars, STARNUMBER);
      }
      break;
    case 4:
      {
	if (*offset==0) {
	  int i;
	  for (i=0;i<GL_XSIZE*MY_YSIZE;i++)
	    *((UNS_8*)pbuf+i) = 0;
	  glfx_init_redfire_palette(palette);
	  *offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE,GL_YSIZE};
	gl_point dp = {0,0};      
	glfx_fire((UNS_8*)pbuf,MY_YSIZE);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
      break;
    case 5:
      {
	if (*offset==0) {
	  int i;
	  for (i=0;i<GL_XSIZE*MY_YSIZE;i++)
	    *((UNS_8*)pbuf+i) = 0;
	  glfx_init_yellowfire_palette(palette);
	  *offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE,GL_YSIZE};
	gl_point dp = {0,0};
	
	glfx_fire2((UNS_8*)pbuf,MY_YSIZE);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
      break;
    case 6:
      {
	if (*offset==0) {
	  int i;
	  for (i=0;i<GL_XSIZE*GL_YSIZE;i++)
	    *((UNS_8*)pbuf+i) = 0;
	  //			for(i=0;i<256;i++)
	  //				costable[i] = (int)( 30 * cos(i * (3.14159/16) ) );
	  glfx_init_radient_palette(palette);
	  pvs.b1 = pvs.b2 = pvs.b3 = pvs.b4 = 0;
	  *offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE,GL_YSIZE};
	gl_point dp = {0,0};
	
	glfx_plasma((UNS_8*)pbuf, costable, &pvs);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
      break;
    case 7:
      gl_clrscr((gl_col*)&glcol_black);
	if (!dirty) {
	  InitGVectors(sword3d,1);
	  dirty = 1;
	}
	UpdateGVectors(512/8,(gl_col*)&glcol_red);
      break;
    defualt:
      break;
    }
}

static void generate_text(UNS_32 effect, CHAR *text, UNS_32 *offset)
{
  switch(effect)
    {
    case 0:
      {
	gl_point p = {0,4};
	gl_col c = {0,255};

	//	gl_clrscr(&glcol_black);
	if (glfx_scrolltext(text,&p,&c,offset,0))
	  *offset = 0;
      }
      break;
    case 1:
      {
	gl_col c = {0,255};
	
	//	gl_clrscr(&glcol_black);
	if (glfx_charsinscroll(text,&c,sintable,offset,0))
	  *offset = 0;
      }
      break;
    case 2:
      {
	gl_col c = {0,255};
	
	//	gl_clrscr(&glcol_black);
	if (glfx_pixsinscroll(text,&c,sintable,offset,0))
	  *offset = 0;
      }
      break;
    default:
      break;
    }
}

const char t1[] = { "Arrow Engineering Sweden proudly presents...                The Bitfire Development Kit!" };
const char t2[] = { "and" };
const char t3[] = { "ARMSCHOOL Workshops v2 2005!                                                            " };
const char t4[] = { "credits..." };
const char t5[] = { "Lead hardware design: Daniel Nilsson                   Additional hardware design: Martin Trojer                   " };
const char t6[] = { "Lead software and FPGA design: Martin Trojer           Additional software design: Daniel Nilsson                  " };
const char t7[] = { "Moral support: Micael Andersson, Leif Hellner, Mattias Hedlund and Marina Ripa                                                                " }; 
const char t8[] = { "                                                       Big shout-out to the whole Arrow ARMSCHOOL Team, you rock!  " };
const char t9[] = { "                                    Let's Bitfire baby!                                                       " };

const char* texts[] = { t1,t2,t3,t4,t5,t6,t7,t8,t9 };

int main(void)
{
  UNS_32 eoffset=0,ectr=0,emode=0;
  UNS_32 soffset=1;
  UNS_32 sctr=0,smode=0;
  int i;

  gl_init();
  gltxt_setfont(font_rom8x8_bits,8,8);
  gl_flipbuf();

  for(i=0;i<256;i++)
    costable[i] = (int)( 30 * cos(i * (3.14159/16) ) );

  for(i=0;i<GL_XSIZE*GL_XBOARD+16;i++)
    sintable[i] = (GL_YSIZE*GL_YBOARD/2)-8+(int)( (GL_YSIZE*GL_YBOARD/2-4) * sin(i * (3.14159/(GL_XSIZE*GL_XBOARD/4)) ) );

  
  while(1)
    {
      int j;

      ectr++;

      if ((ectr%1000)==0) {
	eoffset=0;
	emode = (emode+1)%8;
      }

      generate_effect(emode,&eoffset);
      eoffset++;

      if (soffset==0) {
	sctr = (sctr+1)%9;
	smode = (smode+1)%3;
	soffset=1;
      }
       
      generate_text(smode, texts[sctr], &soffset);
      if (soffset!=0)
	soffset++;      

      gl_flipbuf();
      for (j=0;j<120000;)
	j++;
    }
}

