// FPGA device driver
// mtrojer@arrownordic.com
// $WCREV$

#include "71x_lib.h"
#include "atmosfire_fpga.h"

void fpga_init(void)
{
  GPIO_Config (GPIO0, 0x00FF, GPIO_AF_PP);
  GPIO_Config(GPIO0, 1<<12, GPIO_OUT_PP);    // GPIO0.12: Sync on level high
  GPIO_Config(GPIO2, 1<<12, GPIO_OUT_PP);    // GPIO2.12: Data/Command line
  GPIO_Config(GPIO0, 1<<7, GPIO_OUT_PP);     // BSPI1 SSEL
  GPIO_Config(GPIO1, 1<<8, GPIO_OUT_PP);     // CSEL (should be low when writing)
}

void fpga_command(void)
{
  GPIO_BitWrite(GPIO0,12,1);  // Toggle sync
  GPIO_BitWrite(GPIO0,12,0);
  GPIO_BitWrite(GPIO2,12,1);  // P2.12: 0=Data, 1=Command
}

void fpga_data(void)
{
  GPIO_BitWrite(GPIO0,12,1);  // Toggle sync
  GPIO_BitWrite(GPIO0,12,0);
  GPIO_BitWrite(GPIO2,12,0);  // P2.12: 0=Data, 1=Command
}
