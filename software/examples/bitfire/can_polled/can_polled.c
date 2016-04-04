// CAN Polled test
// (loopback test on one board)
// mtrojer@arrownordic.com
// $WCREV$

#include "lpc2129_can_polled.h"

int main(void)
{
  can_control tx = {2,0x55,1000000,0};
  can_control rx = {1,0xAA,1000000,0};
  UNS_8 txdata[8];
  UNS_8 rxdata[8];
  UNS_8 i,j;
  
  can_polled_set_filters(0);
  can_polled_init(&tx);
  can_polled_init(&rx);

  for (i=0;i<0xFF;i++) {
    can_polled_clear_rxbuf(&rx);
    for (j=0;j<8;j++)
    	rxdata[j]=0xFF;
    for (j=0;j<(i%9);j++)
      txdata[j]=i+j;
    can_polled_send(&tx,(i%9),txdata);
    can_polled_receive(&rx,rxdata);
  }
}
