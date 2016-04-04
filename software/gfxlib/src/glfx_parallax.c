// Bitfire Graphics Library, Parallax
// mtrojer@arrownordic.com
// $WCREV$

#include <math.h>

#include "gfxlib.h"
#include "gfxfx.h"

// TODO: Add documentation
// TODO: Fix x-axis screen overlap.
// TODO: Fix palette

static void vline(INT_16 y1, INT_16 x, UNS_8 color, UNS_8 *pbuf)
{
	INT_32 y,w;

	w=y1*GL_XSIZE*GL_XBOARD+x;
	y=y1;
	while (y<GL_YSIZE*GL_YBOARD)
	{
		if (*(pbuf+w)>0) 
			break;	
		*(pbuf+w)=color;
		y++;
		w+=GL_XSIZE*GL_XBOARD;	
	}
}


float sine_table[451];

static float sine(INT_16 angle)
{ 
	return sine_table[angle]; 
} 

static float cosine(INT_16 angle)
{ 
	return sine_table[angle+90];
} 

static void calc_sine_table(void)
{ 
 	INT_16 ang;
	for (ang=0;ang<451;ang++)
		sine_table[ang] = sin(ang*3.14159/180); 
}

void glfx_parallax(UNS_8 *pbuf, UNS_16 angle)
{
	INT_32 y,x,w;
	
	calc_sine_table();

	for (y=GL_YSIZE*GL_YBOARD;y>=0;y--)
	{
		x=((GL_XSIZE*GL_XBOARD/2)+sine((y*2+angle)%360)*40+cosine((y*2+angle*2)%360)*70);
		for (w=0;w<=7;w++) 
			vline(y,x+w,1+w+16*((y+angle)&7),pbuf);		
		x=((GL_XSIZE*GL_XBOARD/2)+sine((360-y*2+angle*2)%360)*50+cosine((360-y*2+angle)%360)*60);
		for (w=0;w<=7;w++) 
			vline(y,x+w,128+1+w+16*((y+angle)&7),pbuf);		
	}
}
