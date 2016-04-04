// Common SPI
// mtrojer@arrownordic.com
// $WCREV$
//! Common include file for SPI drivers.
//! Defines SPCR_* and SPI_* constants.
//! Defines struct spi_control
//! \page LPC2129_SPI lpc2129_spi.h 
//! \ingroup BSPINC

#ifndef _LPC2129_SPI_H
#define _LPC2129_SPI_H

#include "bitfire_types.h"

#define SPCR_CPHA_0      (0x0 << 3)
#define SPCR_CPHA_1      (0x1 << 3)
#define SPCR_CPOL_0      (0x0 << 4)
#define SPCR_CPOL_1      (0x1 << 4)
#define SPCR_MSTR_MASTER (0x1 << 5)
#define SPCR_MSTR_SLAVE  (0x0 << 5)
#define SPCR_LSBF_LSB    (0x1 << 6)
#define SPCR_LSBF_MSB    (0x0 << 6)
#define SPCR_SPIE_0      (0x0 << 7)
#define SPCR_SPIE_1      (0x1 << 7)

#define SPCCR_FASTEST     8

#define SPI_STATUS_NOT_STARTED 0x00
#define SPI_STATUS_OK          0xFF

typedef struct
{
  UNS_8 port;
  UNS_8 status;
  UNS_8 SPCR;
  UNS_8 SPCCR;
  UNS_8 vecaddr;  // which IRQ vecaddr to use (must be unique!)
} spi_control;


#endif
