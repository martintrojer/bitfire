// Simple Win App
// mtrojer@arrownordic.com
// $WCREV$

#include <windows.h>
#include <math.h>

#include "disp_emu.h"
#include "gfxlib.h"
#include "gfxtxt.h"
#include "gfxfx.h"
#include "font_rom8x8.h"

#include "gfxvec.h"
#include "gfxvec_cube3d.h"
#include "gfxvec_sword3d.h"
#include "gfxvec_football3d.h"
#include "gfxvec_key3d.h"
#include "gfxvec_house3d.h"
#include "gfxvec_star3d.h"
#include "gfxvec_qbinaqb3d.h"

const char *ClsName = "BasicApp";
const char *WndName = "AEB EMU v0.2";

LRESULT CALLBACK WndProcedure(HWND hWnd, UINT uMsg,
			   WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
               LPSTR lpCmdLine, int nCmdShow)
{
	MSG        Msg;
	HWND       hWnd;
	WNDCLASSEX WndClsEx;

	// Create the application window
	WndClsEx.cbSize        = sizeof(WNDCLASSEX);
	WndClsEx.style         = CS_HREDRAW | CS_VREDRAW;
	WndClsEx.lpfnWndProc   = WndProcedure;
	WndClsEx.cbClsExtra    = 0;
	WndClsEx.cbWndExtra    = 0;
	WndClsEx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	WndClsEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
	WndClsEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClsEx.lpszMenuName  = NULL;
	WndClsEx.lpszClassName = ClsName;
	WndClsEx.hInstance     = hInstance;
	WndClsEx.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

	// Register the application
	RegisterClassEx(&WndClsEx);

	// Create the window object
	hWnd = CreateWindow(ClsName,
			  WndName,
			  WS_OVERLAPPEDWINDOW,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  CW_USEDEFAULT,
			  NULL,
			  NULL,
			  hInstance,
			  NULL);
	
	// Find out if the window was created
	if( !hWnd ) // If the window was not created,
		return 0; // stop the application

	// Display the window to the user
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	// Decode and treat the messages
	// as long as the application is running
	while( GetMessage(&Msg, NULL, 0, 0) )
	{
             TranslateMessage(&Msg);
             DispatchMessage(&Msg);
	}

	return (INT) Msg.wParam;
}

void DrawOurGraphics(HWND hWnd);

LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg,
			   WPARAM wParam, LPARAM lParam)
{
    switch(Msg)
    {
	case WM_CREATE:
		SetTimer(hWnd,WM_USER+66,350,(TIMERPROC)NULL);	
		break;
    // If the user wants to close the application	
	case WM_TIMER:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		DrawOurGraphics(hWnd);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
        // then close it
        PostQuitMessage(WM_QUIT);
        break;
    default:
        // Process the left-over messages
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    // If something was not done, let it go
    return 0;
}

DispEmu myDisp;

INT_8 sintable[GL_XSIZE*GL_XBOARD+16];
INT_8 sintable_dbl[GL_XSIZE*GL_XBOARD+16];
INT_8 costable[256];

#define STARNUMBER 32*25
glfx_star stars[STARNUMBER];

UNS_8 pbuf[GL_XSIZE*GL_XBOARD][GL_YSIZE*GL_YBOARD];
gl_col palette[256];
glfx_plasmavars pvs;

BOOL_8 dirty = 0;

static void generate_effect(UNS_32 effect, UNS_32 *offset)
{
	effect=0;
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
		gl_point dp = {0,2};
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
  case 6: // star3d
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

const CHAR text[] = { " ÿArrow Northern Europe - Arrow Nordic - Arrow Sweden - Arrow Engineering @ Electronix Scandinavia 2006                         "\
		      "ÿArrow Electronics is a major global provider of products, services, and solutions to industrial and commercial users of electronic components and computer products.              "\
		      "ÿArrow provides specialized services and expertise at each stage of a product's life cycle, accelerating and simplifying the journey from concept to market. Each product's journey begins with the technology innovations of Arrow's suppliers who introduce thousands of new components and computer products each month. Arrow's customers use that technology to design and manufacture products from home security systems to global telecommunications networks.                 "\
		      "ÿThis demo runs on 3x3 Bitfire development kits, for more information visit:    www.bitfire.se ÿ                     "\
		      "ÿBitfire credits...                 "\
		      "ÿHardware design: Daniel Nilsson, Martin Trojer                         "\
		      "Software and FPGA design: Martin Trojer, Daniel Nilsson                "\
		      "ÿwww.arrowne.com   www.bitfire.se    www.arrowne.com    www.bitfire.se   www.arrowne.com   www.bitfire.se         "\
		      "Arrow - the clear #1        Arrow - the clear #1        Arrow - the clear #1        Arrow - the clear #1         " };


const scroll_cmd scmds[] = { {{0,255},0,5,1}, 
							 {{20,200},0,0,2}, 
                             {{40,200},0,0,3}, 
                             {{60,225},0,1,2}, {{60,225},200,1,2},
                             {{20,140},0,2,0}, 
                             {{20,140},0,4,3}, 
							{{120,255},0,5,1}};

UNS_32 eoffset=0,ectr=0,emode=0;
void DrawOurGraphics(HWND hWnd)
{
	// get "device context" - Detta är beskrivningen på vår fönster yta
	HDC hdc = GetDC (hWnd);

	int i;

	for(i=0;i<256;i++)
		costable[i] = (int)( 30 * cos(i * (3.14159/32) ) );

	gltxt_setfont(font_rom8x8_bits,8,8);

    {
		ectr++;
		if ((ectr%1000)==0) {
			eoffset=0;
			emode = (emode+1)%8;
			dirty = 0;
		}

		generate_effect(emode,&eoffset);
		eoffset++;
		
		gltxt_scroller(text,scmds);

		myDisp.sethdc(hdc);
		myDisp.updatescr();

	}
}
	
/*	
#define FONT_XSIZE 8
#define FONT_YSIZE 8


UNS_32 val1 = 0;						// scrolling
UNS_32 offset = 0;						// patterns
BOOL_8 dirty=0;							// vectors



void DrawOurGraphics(HWND hWnd)
{
	// get "device context" - Detta är beskrivningen på vår fönster yta
	HDC hdc = GetDC (hWnd);
	int i;

			for(i=0;i<256;i++)
				mycostable[i] = (int)( 30 * cos(i * (3.14159/32) ) );

#if 0
	{	// ************** Lines, Circles and Ellipses ********************
		gl_point p1 = {10,10};
		gl_point p2 = {50,20};
//		gl_line(&p1,&p2,(gl_col*)&glcol_green);
//		gl_circle(&p2,20,(gl_col*)&glcol_green);
		gl_ellipse(&p2,25,10,(gl_col*)&glcol_green);
	}
#endif

#if 0
	{	// ************** SIMPLE PATTERNS ********************
//		glfx_pattern2(&offset);
		glfx_pattern3(&offset, mycostable, 256);
		offset++;
	}
#endif

#if 0
	{	// ************** STARFIELD ********************
		int i;
		if (offset!=0) 
			for (i=0;i<STARNUMBER;i++)
				stars[i].rz = stars[i].sx = stars[i].sy = 0;
		glfx_3dstar((glfx_star*)stars, STARNUMBER);
	}
#endif

#if 0
	{	// ************** Fractals ********************
		glfx_mandel(96,0);
//		glfx_julia(96,10);
	}
#endif

#if 0
	{	// ************** 3D Graphics ********************
		gl_clrscr((gl_col*)&glcol_black);

		if (!dirty) {
//			InitGVectors(cube3d,1);
//			InitGVectors(sword3d,1);
			InitGVectors(football3d,1);
//			InitGVectors(flag3d,1);
			dirty = 1;
		}
//		UpdateGVectors(512/18,(gl_col*)&glcol_green);
		UpdateGVectors(512/6,(gl_col*)&glcol_green);
	}
#endif

#if 0
	{	// *********************** FIRE *************************
		if (offset==0) {
			int i;
			for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
				*((UNS_8*)pbuf+i) = 0;
			glfx_init_redfire_palette(palette);
			offset=1;
		}
		gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
		gl_point dp = {0,2};

		glfx_fire((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
		gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
	}
#endif

#if 0
	{	// *********************** FIRE2 *************************
		if (offset==0) {
			int i;
			for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
				*((UNS_8*)pbuf+i) = 0;
			glfx_init_yellowfire_palette(palette);
//			offset=1;
		}
		gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
		gl_point dp = {0,0};

		glfx_fire2((UNS_8*)pbuf,GL_YSIZE*GL_YBOARD);
		gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
	}
#endif

#if	0
	{	// *********************** PLASMA *************************
		if (offset==0) {
			int i;
//			for (i=0;GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
//				*((UNS_8*)pbuf+i) = 0;
			for(i=0;i<256;i++)
				mycostable[i] = (int)(30 * cos(i * (3.14159/32) ) );
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

#if	0
	{	// *********************** PARALLAX *************************
		if (offset==0) {
			glfx_init_radient_palette(palette);
		}

		int i;
		for (i=0;i<GL_XSIZE*GL_XBOARD*GL_YSIZE*GL_YBOARD;i++)
			*((UNS_8*)pbuf+i) = 0;

		gl_rect srect = {0,0,GL_XSIZE*GL_XBOARD,GL_YSIZE*GL_YBOARD};
		gl_point dp = {0,0};
		
		glfx_parallax((UNS_8 *)pbuf,offset);

		gl_pblit((UNS_8*)pbuf, &srect, &dp, palette);
		offset=(offset+4)%360;
	}
#endif

#if 0
	{	// ************ PRINTTEXT ******************
		gltxt_setfont(font_rom8x8_bits,8,8);
		gl_point p = {0,0};
		gl_col c = {255,255};
		gltxt_printtext("ARM",&p,&c);
	}
#endif
	
#if 0
	{	// ************ SCROLLTEXT ******************
		gltxt_setfont(font_rom8x8_bits,8,8);
		gl_point p = {0,10};
		gl_col c = {255,255};

		val1 -= 1;
		glfx_scrolltext("ARMSCHooL 2oo5 -- l337 waRez",&p,(gl_col*)&glcol_black,&val1,1);
		if (glfx_scrolltext("ARMSCHooL 2oo5 -- l337 waRez",&p,(gl_col*)&c,&val1,1))
			val1 = 0;
	}
#endif

#if 1
	{	// ************ SINSCROLL ******************
		if (offset==0) {
			int i; 
			for(i=0;i<GL_XSIZE*GL_XBOARD+FONT_XSIZE;i++)
				mysintable[i] = (GL_YSIZE*GL_YBOARD/2)-8+(int)( (GL_YSIZE*GL_YBOARD/2-8) * sin(i * (3.14159/(GL_XSIZE*GL_XBOARD/4)) ) );
			offset=1;
		}

		gltxt_setfont(font_rom8x8_bits,8,8);
		gl_point p = {0,40};
		gl_col c = {255,0};

		 val1 -= 1;
		// gl_clrscr((gl_col*)&glcol_black);

		glfx_pixsinscroll("ARMSCHooL 2oo5 -- l337 waRez",(gl_col*)&glcol_black,mysintable,&val1,1);
		if (glfx_pixsinscroll("ARMSCHooL 2oo5 -- l337 waRez",&c,mysintable,&val1,1))
			val1 = 0;
	}
#endif

	myDisp.sethdc(hdc);
	myDisp.updatescr();

//	Sleep(100);
}
*/