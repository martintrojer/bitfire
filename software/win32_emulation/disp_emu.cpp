// Emulate the Display on the Kit
// mtrojer@arrownordic.com
// $WCREV$

#include "disp_emu.h"

gl_col myScreenBuffer[GL_XBOARD][GL_YBOARD][GL_XSIZE][GL_YSIZE];

void DispEmu::sethdc(HDC ahdc)
{
	myHDC = ahdc;
}

void DispEmu::updatescr(void)
{
	int x,y,xb,yb;

	// Clear screen
	HBRUSH blackbrush = CreateSolidBrush(COLORREF(RGB(0,0,0)));
	RECT bg = {0,0,1050,640};
	FillRect(myHDC,&bg,blackbrush);
	DeleteObject(blackbrush);

	// Write pixels
	for (yb=0;yb<GL_YBOARD;yb++)
	{
		for (xb=0;xb<GL_XBOARD;xb++)
		{
			for (y=0;y<GL_YSIZE;y++)
			{
				for (x=0;x<GL_XSIZE;x++)
				{
					if (myScreenBuffer[xb][yb][x][y].r==0 && myScreenBuffer[xb][yb][x][y].g==0)
						continue;

					HBRUSH redbrush = CreateSolidBrush(COLORREF(RGB(myScreenBuffer[xb][yb][x][y].r,0,0)));
					HBRUSH greenbrush = CreateSolidBrush(COLORREF(RGB(0,myScreenBuffer[xb][yb][x][y].g,0)));

					int px = xb*GL_XSIZE*(PIX_X*2+X_PIX_SPACE)+xb*X_BOARD_SPACE+x*(PIX_X*2+X_PIX_SPACE);
					int py = yb*GL_YSIZE*(PIX_Y+Y_PIX_SPACE)+yb*Y_BOARD_SPACE+y*(PIX_Y+Y_PIX_SPACE);

					RECT rr = {px,py,px+PIX_X,py+PIX_Y};
					RECT gr = {px+PIX_X,py,px+2*PIX_X,py+PIX_Y};

					FillRect(myHDC, &rr, redbrush);
					FillRect(myHDC, &gr, greenbrush);

					DeleteObject(redbrush);
					DeleteObject(greenbrush);
				}
			}
		}
	}
}

#define ME_X 1
#define ME_Y 2

void setpixel(int x, int y, gl_col *val)
{
	// one screen
//	if (x>=ME_X*GL_XSIZE && x<(ME_X+1)*GL_XSIZE && y>=ME_Y*GL_YSIZE && y<(ME_Y+1)*GL_YSIZE)
//		myScreenBuffer[x/GL_XSIZE][y/GL_YSIZE][x%GL_XSIZE][y%GL_YSIZE] = *val;
	
	// all screens
	if (x>=0 && x<GL_XSIZE*GL_XBOARD && y>=0 && y<GL_YSIZE*GL_YBOARD)
		myScreenBuffer[x/GL_XSIZE][y/GL_YSIZE][x%GL_XSIZE][y%GL_YSIZE] = *val;
}

void bsp_setpixelxy(UNS_16 x, UNS_16 y, gl_col *col)
{
	setpixel(x,y,col);
}

void bsp_setpixel(gl_point *pos, gl_col *col)
{
	setpixel(pos->x,pos->y,col);
}

void bsp_init_gfx(void)
{
}

void bsp_flipbuf(void)
{
}

void bsp_getid(gl_point *idx)
{
	idx->x = ME_X;
	idx->y = ME_Y;
}