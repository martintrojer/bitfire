// Bitfire Graphics Library
// mtrojer@arrownordic.com
// $WCREV$

#include "gfxlib.h"

void gl_cliprect(gl_rect *rect)
{
  //	if (rect->left<0)
  //		rect->left=0;
  //	if (rect->top<0)
  //		rect->top=0;
	if (rect->right>=GL_XSIZE*GL_XBOARD)
		rect->right=(GL_XSIZE*GL_XBOARD)-1;
	if (rect->bottom>=GL_YSIZE*GL_YBOARD)
		rect->bottom=(GL_YSIZE*GL_YBOARD)-1;
}
