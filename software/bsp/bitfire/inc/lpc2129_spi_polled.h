// Polled SPI driver
// mtrojer@arrownordic.com
// $WCREV$
//! Polled SPI driver
//! \page LPC2129_SPI_POLLED lpc2129_spi_polled.h 
//! \ingroup BSPINC
//!

#ifndef _LPC2129_SPI_POLLED_H
#define _LPC2129_SPI_POLLED_H

#include "lpc2129_spi.h"

//! \defgroup lpc2129_spi_polled lpc2129_spi_polled

//! Initializes the SPI peripheral
//! @param *sc Pointer to initialized spi_control struct.
//! Will update the status field.
//!
//! Example usage:
//! <pre>
//!  spi_control sc = {0, 0,                          \n
//!		    SPCR_MSTR_MASTER | SPCR_LSBF_LSB, \n
//!		    SPCCR_FASTEST,0};                 \n
//!  spi_polled_init(&sc);                            \n
//! </pre>
//! \ingroup lpc2129_spi_polled
void spi_polled_init(spi_control *sc);

//! Writes 8 bits to the SPI peripheral
//! @param *sc Pointer to initialized spi_control struct.
//! You must call spi_polled_init() first.
//! @param ch Containing the 8 bits to be sent.
//! \ingroup lpc2129_spi_polled
UNS_8 spi_polled_putchar (spi_control *sc, UNS_8 ch);

//! Reads 8 bits from the SPI periferal.
//! NOT IMPLEMENTED!
//! @param *sc Pointer to initialized spi_control struct.
//! \ingroup lpc2129_spi_polled
UNS_8 spi_polled_getchar (spi_control *sc);

#endif
