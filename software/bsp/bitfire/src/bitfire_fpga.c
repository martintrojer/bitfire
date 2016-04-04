// FPGA device driver
// mtrojer@arrownordic.com
// $WCREV$

#include "lpc21xx.h"
#include "bitfire_fpga.h"

void fpga_init(void)
{
  PINSEL0 = PINSEL0 & 0xf0ffffff;  // P0.12: Sync on level high
  IODIR0 = IODIR0 | 0x00003000;    // P0.13: 0=Data, 1=Command
}

void fpga_command(void)
{
  IOSET0 = 0x00001000;   // Toggle Sync
  IOCLR0 = 0x00001000;
  IOSET0 = 0x00002000;
}

void fpga_data(void)
{
  IOSET0 = 0x00001000;   // Toggle Sync
  IOCLR0 = 0x00001000;
  IOCLR0 = 0x00002000;
}
