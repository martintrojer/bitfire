// Polled Timer test
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "lpc21xx.h"

int main(void)
{
  UNS_32 i,data[10]={0,0,0,0,0,0,0,0,0,0};

  T0TCR = 0;       // Disable timer 0
  T0PC  = 0;       // No division
  T0TC  = 0;       // Reset Timer Counter
  T0MR0 = 0xFFFFFFFF;     // Count up to this value
  T0MCR = 4;       // Stop on MR0
  T0CCR = 0;       // Disable capture
  T0EMR = 0;       // No external match output
  T0TCR = 1;       // Enable timer 0

  for (i=0;i<10;i++)
    data[i] = T0TC;

}
