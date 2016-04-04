// GPIO test
// mtrojer@arrownordic.com
// $WCREV$

#include "lpc21xx.h"

int main(void)
{
#if 0
  IODIR0 = 0x00002050;
  IOSET0 = 0x00002000;
  IOCLR0 = 0x00002000;

  IOSET0 = 0x00000050;
  IOCLR0 = 0x00000050;
  IOSET0 = 0x00000050;  
#endif

  {
    int i,inc=0;
    IODIR0 = 0x01000000;
    while(1)
      {
	if (inc%2)
	  IOSET0 = 0x01000000;
	else
	  IOCLR0 = 0x01000000;
	inc++;
	//	for(i=0;i<100;)
	//	  i++;
      }
  }
}
