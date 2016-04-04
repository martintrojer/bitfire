
#include "bitfire_types.h"
#include "gfxlib.h"

int main(void)
{
  
  void *mem;
  gl_init();
  gl_col col = {100,100};

  // Add your code here
  gl_setpixelxy(0,0,&col);
  
  while(1)
	  ;

}

