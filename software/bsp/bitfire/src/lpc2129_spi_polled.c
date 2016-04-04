// SPI polled
// mtrojer@arrownordic.com
// $WCREV$

#include "lpc21xx.h"
#include "lpc2129_spi_polled.h"

void spi_polled_init(spi_control *sc)
{
  if (sc->port==0) {
    PINSEL0 = PINSEL0 | 0x00005500;
    S0SPCCR = sc->SPCCR;             // 8
    S0SPCR = sc->SPCR;               // 0x60
  }

  if (sc->port==1) {
    PINSEL1 = PINSEL1 | 0x000002A8;
    S1SPCCR = sc->SPCCR;
    S1SPCR = sc->SPCR;
  }

  sc->status = SPI_STATUS_OK;
}

UNS_8 spi_polled_putchar (spi_control *sc, UNS_8 ch)
{
  UNS_8 status = 0;

  if (sc->status!=SPI_STATUS_OK)
    return 0;

  if (sc->port==0) {  
    S0SPDR = ch;
    while(!(status & 0x80))  // Wait for complete flag
      status = S0SPSR;
    return ((S0SPSR&0x78) ? status : 0);
  }

  if (sc->port==1) {  
    S1SPDR = ch;
    while(!(status & 0x80))  // Wait for complete flag
      status = S1SPSR;
    return ((S1SPSR&0x78) ? status : 0);
  }
}
