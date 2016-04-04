// SPI Polled test
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "lpc2129_spi_polled.h"

int main(void)
{
  UNS_8 inc=0;
  spi_control sc = {0, 0,
		    SPCR_MSTR_MASTER | SPCR_LSBF_LSB,
		    SPCCR_FASTEST,0}; 

  spi_polled_init(&sc);

  while(1) {
    spi_polled_putchar(&sc,inc++);
  }
}
