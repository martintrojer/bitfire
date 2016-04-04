// SPI pixeltest
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "lpc2129_spi_polled.h"
#include "bitfire_fpga.h"

#define X_SIZE 40
#define Y_SIZE 16

spi_control sc = {0, 0,
		  SPCR_MSTR_MASTER | SPCR_LSBF_MSB,
		  SPCCR_FASTEST,0}; 

void setpixel(UNS_8 x, UNS_8 y, UNS_8 r, UNS_8 g)
{
  UNS_16 addr = x+y*X_SIZE;
  UNS_8 addrlo = (UNS_8) addr;
  UNS_8 addrhi = (UNS_8) (addr>>8);

  fpga_data();
  spi_polled_putchar(&sc, addrlo);
  spi_polled_putchar(&sc,addrhi);
  spi_polled_putchar(&sc, r);
  spi_polled_putchar(&sc, g);
}

int main(void)
{
  UNS_8 inc=0,x,y;

  spi_polled_init(&sc);
  fpga_init();
  fpga_command();
  spi_polled_putchar(&sc, FPGACMD_shdnn_muxsup_1);
  fpga_command();
  spi_polled_putchar(&sc, FPGACMD_shdn_ledsup_0);
  fpga_command();
  spi_polled_putchar(&sc, FPGACMD_DIMMER_PRE5);  
  fpga_command();
  spi_polled_putchar(&sc, FPGACMD_SHOW_BUF_0);  
  fpga_command();
  spi_polled_putchar(&sc, FPGACMD_WORK_BUF_0);  
  
  while(1) {
    for (y=0;y<Y_SIZE;y++) {
      for (x=0;x<X_SIZE;x++) {
	setpixel(x,y,255-x*6,inc);
      }
    }
    inc++;
  }

  inc=12;
}
