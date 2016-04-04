// Multiple bitfires demo
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
#include "gfxvec_sword3d.h"
#include "gfxvec_football3d.h"
#include "gfxvec_star3d.h"

#include "lpc21xx.h"
#include "lpc2129_can_polled.h"

const INT_8 costable[256] = {30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29, 30, 29, 27, 24, 21, 16, 11, 5, 0, -5, -11, -16, -21, -24, -27, -29, -30, -29, -27, -24, -21, -16, -11, -5, 0, 5, 11, 16, 21, 24, 27, 29};

#define STARNUMBER 90
glfx_star stars[STARNUMBER];

UNS_8 pbuf[GL_XSIZE*GL_XBOARD][GL_YSIZE*GL_YBOARD];
gl_col palette[256];
glfx_plasmavars pvs;

BOOL_8 dirty = 0;

static void generate_effect(UNS_32 effect, UNS_32 *offset)
{
  switch(effect)
    {
    case 0:	// cube3d
      gl_clrscr((gl_col*)&glcol_black);
      if (!dirty) {
	InitGVectors(cube3d,1);
	dirty = 1;
      }
      UpdateGVectors(512/5,(gl_col*)&glcol_red);
      break;
    case 1:	// 3dstar
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
    case 2: // football3d
      gl_clrscr((gl_col*)&glcol_black);
      if (!dirty) {
	InitGVectors(football3d,1);
	dirty = 1;
      }
      UpdateGVectors(512/6,(gl_col*)&glcol_red);
      break;
    case 3:	// fire
      {
	if (*offset==0) {
	  int i;
	  for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
	    *((UNS_8*)pbuf+i) = 0;
	  glfx_init_redfire_palette(palette);
	  *offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_point dp = {0,0};
	gl_rect brect = {0,GL_YSIZE*GL_YBOARD-2,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_col c = {25,25};
		
	glfx_fire((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
	gl_fillrect(&brect,&c);
      }
      break;
    case 4: // sword3d
      gl_clrscr((gl_col*)&glcol_black);
      if (!dirty) {
	InitGVectors(sword3d,1);
	dirty = 1;
      }
      UpdateGVectors(512/4,(gl_col*)&glcol_red);
      break;
    case 5:	// fire2
      {
	if (*offset==0) {
	  int i;
	  for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
	    *((UNS_8*)pbuf+i) = 0;
	  glfx_init_yellowfire_palette(palette);
	  *offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
	gl_point dp = {0,0};

	glfx_fire2((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
      break;
    case 6:	// star3d
      gl_clrscr((gl_col*)&glcol_black);
      if (!dirty) {
	InitGVectors(star3d,1);
	dirty = 1;
      }
      UpdateGVectors(512/4,(gl_col*)&glcol_red);
      break;
    case 7:	// plasma
      {
	if (*offset==0) {
	  glfx_init_radient_palette(palette);
	  pvs.b1 = pvs.b2 = pvs.b3 = pvs.b4 = 0;
	  *offset=1;
	}
	gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD-1,GL_YSIZE*GL_YBOARD-1};
	gl_point dp = {0,0};

	glfx_plasma((UNS_8*)pbuf, costable, &pvs);
	gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
      }
      break;
    default:
      break;
    }
}

const CHAR text[] = { " ÿArrow Northern Europe - Arrow Nordic - Arrow Sweden - Arrow Engineering                            "\
		      "ÿArrow Electronics is a major global provider of products, services, and solutions to industrial and commercial users of electronic components and computer products.              "\
		      "ÿArrow provides specialized services and expertise at each stage of a product's life cycle, accelerating and simplifying the journey from concept to market. Each product's journey begins with the technology innovations of Arrow's suppliers who introduce thousands of new components and computer products each month. Arrow's customers use that technology to design and manufacture products from home security systems to global telecommunications networks.                 "\
		      "ÿThis demo runs on 3x3 Bitfire development kits, for more information visit:    www.bitfire.se ÿ                     "\
		      "ÿBitfire devevlopment kit credits...                 "\
		      "ÿHardware design: Daniel Nilsson, Martin Trojer                         "\
		      "Software and FPGA design: Martin Trojer, Daniel Nilsson                "\
		      "ÿwww.arrowne.com   www.bitfire.se    www.arrowne.com    www.bitfire.se   www.arrowne.com   www.bitfire.se         "\
		      "Arrow - the clear #1        Arrow - the clear #1        Arrow - the clear #1        Arrow - the clear #1         " };


const scroll_cmd scmds[] = { {{0,255},0,3,1}, 
			     {{20,200},0,0,2}, 
                             {{40,200},0,0,2}, 
                             {{60,225},0,1,1}, {{60,225},300,1,1},
                             {{20,140},0,2,0}, 
                             {{20,140},0,4,2}, 
			     {{120,255},0,5,0}};

int main(void)
{
  int i;
  UNS_32 eoffset=0, ectr=0, emode=0;
  can_control tx = {2,0x55,1000000,0};
  can_control rx = {1,0xAA,1000000,0};
  UNS_8 txdata[8];
  UNS_8 rxdata[8];
  gl_point mine_idx;
  UNS_32 wait,maxwait=0x00200000;

  can_polled_set_filters(0);
  can_polled_init(&tx);
  can_polled_init(&rx);
  gl_getid(&mine_idx);
  
  txdata[0] = 0xAA;

  if (mine_idx.x==0 && mine_idx.y==0) {
    txdata[0]=0;
    can_polled_send(&tx,1,txdata);
  }
  else {
    while(1) {
      can_polled_clear_rxbuf(&rx);
      can_polled_receive(&rx,rxdata);
      can_polled_clear_rxbuf(&rx);
      if (rxdata[0]==0)
	break;
    }
  }

  for (i=0;i<STARNUMBER;i++)
    stars[i].rz = stars[i].sx = stars[i].sy = 0;

  for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
    *((UNS_8*)pbuf+i) = 0;

  gltxt_setfont(font_rom8x8_bits,8,8);
  gl_init();
  gl_flipbuf();

  T0TCR = 0;       // Disable timer 0
  T0PC  = 0;       // No division
  T0TC  = 0;       // Reset Timer Counter
  T0MR0 = 0xFFFFFFFF;     // Count up to this value
  T0MCR = 4;       // Stop on MR0
  T0CCR = 0;       // Disable capture
  T0EMR = 0;       // No external match output
  T0TCR = 1;       // Enable timer 0

  while(1) {

    T0TC  = 0;       // Reset Timer Counter

    ectr++;
    if ((ectr%1000)==0) {
      eoffset=0;
      emode = (emode+1)%8;
      dirty = 0;
    }
    
    generate_effect(emode,&eoffset);
    eoffset++;
    
    gltxt_scroller(text,scmds);

    if (mine_idx.x==0 && mine_idx.y==0) { // MASTER
      wait = T0TC;
      if (wait>maxwait)
	maxwait=wait+1024;

      while(T0TC<maxwait);
      
      txdata[0]=1;
      can_polled_send(&tx,1,txdata);
    }
    else {                                // SLAVE
      can_polled_receive(&rx,rxdata);
      can_polled_clear_rxbuf(&rx);
    }

    gl_flipbuf();   // flipbuf flicking
    wait = T0TC+240000;
    while(T0TC<wait);
  }
}

