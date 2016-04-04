//==========================================================================
//
//      io/serial/arm/str7xx/str7xx_serial.c
//
//      ST STR7XX/Atmosfire Serial I/O Interface Module (interrupt driven)
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):     mtrojer
// Date:          2007-03-09
// Purpose:       ST STR7XX/Atmosfire Serial I/O module (interrupt driven version)
// Description: 
//
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/system.h>
#include <pkgconf/io_serial.h>
#include <pkgconf/io.h>
#include <pkgconf/kernel.h>

#include <cyg/io/io.h>
#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/io/devtab.h>
#include <cyg/io/serial.h>
#include <cyg/infra/diag.h>

#ifdef CYGPKG_IO_SERIAL_ARM_STR7XX

#include "str7xx_serial.h"

typedef struct str7xx_serial_info {
    CYG_WORD      id;
    CYG_WORD       int_num;
    cyg_interrupt  serial_interrupt;
    cyg_handle_t   serial_interrupt_handle;
} str7xx_serial_info;

static bool str7xx_serial_init(struct cyg_devtab_entry *tab);
static bool str7xx_serial_putc(serial_channel *chan, unsigned char c);
static Cyg_ErrNo str7xx_serial_lookup(struct cyg_devtab_entry **tab, 
                                    struct cyg_devtab_entry *sub_tab,
                                    const char *name);
static unsigned char str7xx_serial_getc(serial_channel *chan);
static Cyg_ErrNo str7xx_serial_set_config(serial_channel *chan, cyg_uint32 key,
                                        const void *xbuf, cyg_uint32 *len);
static void str7xx_serial_start_xmit(serial_channel *chan);
static void str7xx_serial_stop_xmit(serial_channel *chan);

static cyg_uint32 str7xx_serial_ISR(cyg_vector_t vector, cyg_addrword_t data);
static void       str7xx_serial_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data);

static SERIAL_FUNS(str7xx_serial_funs, 
                   str7xx_serial_putc, 
                   str7xx_serial_getc,
                   str7xx_serial_set_config,
                   str7xx_serial_start_xmit,
                   str7xx_serial_stop_xmit
    );

#ifdef CYGPKG_IO_SERIAL_ARM_STR7XX_SERIAL0
static str7xx_serial_info str7xx_serial_info0 = {0, CYGNUM_HAL_INTERRUPT_UART2IRQ};
#if CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL0_BUFSIZE > 0
static unsigned char str7xx_serial_out_buf0[CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL0_BUFSIZE];
static unsigned char str7xx_serial_in_buf0[CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL0_BUFSIZE];

static SERIAL_CHANNEL_USING_INTERRUPTS(str7xx_serial_channel0,
                                       str7xx_serial_funs, 
                                       str7xx_serial_info0,
                                       CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL0_BAUD),
                                       CYG_SERIAL_STOP_DEFAULT,
                                       CYG_SERIAL_PARITY_DEFAULT,
                                       CYG_SERIAL_WORD_LENGTH_DEFAULT,
                                       CYG_SERIAL_FLAGS_DEFAULT,
                                       &str7xx_serial_out_buf0[0], sizeof(str7xx_serial_out_buf0),
                                       &str7xx_serial_in_buf0[0], sizeof(str7xx_serial_in_buf0)
    );
#else
static SERIAL_CHANNEL(str7xx_serial_channel0,
                      str7xx_serial_funs, 
                      str7xx_serial_info0,
                      CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL0_BAUD),
                      CYG_SERIAL_STOP_DEFAULT,
                      CYG_SERIAL_PARITY_DEFAULT,
                      CYG_SERIAL_WORD_LENGTH_DEFAULT,
                      CYG_SERIAL_FLAGS_DEFAULT
    );
#endif

DEVTAB_ENTRY(str7xx_serial_io0, 
             CYGDAT_IO_SERIAL_ARM_STR7XX_SERIAL0_NAME,
             0,                     // Does not depend on a lower level interface
             &cyg_io_serial_devio, 
             str7xx_serial_init, 
             str7xx_serial_lookup,     // Serial driver may need initializing
             &str7xx_serial_channel0
    );
#endif //  CYGPKG_IO_SERIAL_ARM_STR7XX_SERIAL1

#ifdef CYGPKG_IO_SERIAL_ARM_STR7XX_SERIAL1
static str7xx_serial_info str7xx_serial_info1 = {1, CYGNUM_HAL_INTERRUPT_UART3IRQ};
#if CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL1_BUFSIZE > 0
static unsigned char str7xx_serial_out_buf1[CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL1_BUFSIZE];
static unsigned char str7xx_serial_in_buf1[CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL1_BUFSIZE];

static SERIAL_CHANNEL_USING_INTERRUPTS(str7xx_serial_channel1,
                                       str7xx_serial_funs, 
                                       str7xx_serial_info1,
                                       CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL1_BAUD),
                                       CYG_SERIAL_STOP_DEFAULT,
                                       CYG_SERIAL_PARITY_DEFAULT,
                                       CYG_SERIAL_WORD_LENGTH_DEFAULT,
                                       CYG_SERIAL_FLAGS_DEFAULT,
                                       &str7xx_serial_out_buf1[0], sizeof(str7xx_serial_out_buf1),
                                       &str7xx_serial_in_buf1[0], sizeof(str7xx_serial_in_buf1)
    );
#else
static SERIAL_CHANNEL(str7xx_serial_channel1,
                      str7xx_serial_funs, 
                      str7xx_serial_info1,
                      CYG_SERIAL_BAUD_RATE(CYGNUM_IO_SERIAL_ARM_STR7XX_SERIAL1_BAUD),
                      CYG_SERIAL_STOP_DEFAULT,
                      CYG_SERIAL_PARITY_DEFAULT,
                      CYG_SERIAL_WORD_LENGTH_DEFAULT,
                      CYG_SERIAL_FLAGS_DEFAULT
    );
#endif

DEVTAB_ENTRY(str7xx_serial_io1, 
             CYGDAT_IO_SERIAL_ARM_STR7XX_SERIAL1_NAME,
             0,                     // Does not depend on a lower level interface
             &cyg_io_serial_devio, 
             str7xx_serial_init, 
             str7xx_serial_lookup,     // Serial driver may need initializing
             &str7xx_serial_channel1
    );
#endif //  CYGPKG_IO_SERIAL_ARM_STR7XX_SERIAL1

// Internal function to actually configure the hardware to desired baud rate, etc.
static bool
str7xx_serial_config_port(serial_channel *chan, cyg_serial_info_t *new_config, bool init)
{
    str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
    cyg_uint8 id = str7xx_chan->id;
    cyg_uint32 parity = new_config->parity;
    cyg_uint32 word_length = new_config->word_length-CYGNUM_SERIAL_WORD_LENGTH_5;
    cyg_uint32 stop_bits = new_config->stop;
    cyg_uint32 baud = new_config->baud;

    if ((word_length == 0xFF) ||
        (parity == 0xFF) ||
        (stop_bits == 0xFF)) {
        return false;  // Unsupported configuration
    }

    if (id==0) {
    // Setup UART2
      GPIO_Config(GPIO0, UART2_Tx_Pin, GPIO_AF_PP);
      GPIO_Config(GPIO0, UART2_Rx_Pin, GPIO_IN_TRI_CMOS);
      UART_Init(UART2);
      UART_OnOffConfig(UART2, ENABLE); 
      UART_FifoConfig (UART2, DISABLE);
      UART_FifoReset  (UART2 , UART_RxFIFO);
      UART_FifoReset  (UART2 , UART_TxFIFO);
      UART_LoopBackConfig(UART2 , DISABLE);
      UART_Config(UART2,38400,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
      UART_ItConfig(UART2,UART_RxBufFull, ENABLE);
      UART_RxConfig(UART2 ,ENABLE);
    }
    else if (id==1) {
    // Setup UART3 
      GPIO_Config(GPIO0, UART3_Tx_Pin, GPIO_AF_PP);
      GPIO_Config(GPIO0, UART3_Rx_Pin, GPIO_IN_TRI_CMOS);
      UART_Init(UART3);
      UART_OnOffConfig(UART3, ENABLE); 
      UART_FifoConfig (UART3, DISABLE);
      UART_FifoReset  (UART3 , UART_RxFIFO);
      UART_FifoReset  (UART3 , UART_TxFIFO);
      UART_LoopBackConfig(UART3 , DISABLE);
      UART_Config(UART3,38400,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
      UART_ItConfig(UART3,UART_RxBufFull, ENABLE);
      UART_RxConfig(UART3 ,ENABLE);
    }

    return true;
}

// Function to initialize the device.  Called at bootstrap time.
static bool 
str7xx_serial_init(struct cyg_devtab_entry *tab)
{
    serial_channel *chan = (serial_channel *)tab->priv;
    str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
    int res;

#ifdef CYGDBG_IO_INIT
    diag_printf("STR7XX SERIAL init - dev: %x.%d\n", str7xx_chan->id, str7xx_chan->int_num);
#endif
    (chan->callbacks->serial_init)(chan);  // Really only required for interrupt driven devices
    if (chan->out_cbuf.len != 0) {
        cyg_drv_interrupt_create(str7xx_chan->int_num,
                                 4,                      // Priority
                                 (cyg_addrword_t)chan,   // Data item passed to interrupt handler
                                 str7xx_serial_ISR,
                                 str7xx_serial_DSR,
                                 &str7xx_chan->serial_interrupt_handle,
                                 &str7xx_chan->serial_interrupt);
        cyg_drv_interrupt_attach(str7xx_chan->serial_interrupt_handle);
        cyg_drv_interrupt_unmask(str7xx_chan->int_num);
    }
    res = str7xx_serial_config_port(chan, &chan->config, true);
    return res;
}

// This routine is called when the device is "looked" up (i.e. attached)
static Cyg_ErrNo 
str7xx_serial_lookup(struct cyg_devtab_entry **tab, 
                  struct cyg_devtab_entry *sub_tab,
                  const char *name)
{
    serial_channel *chan = (serial_channel *)(*tab)->priv;

    (chan->callbacks->serial_init)(chan);  // Really only required for interrupt driven devices
    return ENOERR;
}

// Send a character to the device output buffer.
// Return 'true' if character is sent to device
static bool
str7xx_serial_putc(serial_channel *chan, unsigned char c)
{
    str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
    cyg_uint8 id = str7xx_chan->id;
	char lf='\r';
	
	if (c=='\n') {
		if (id==0)
			UART_ByteSend(UART2, &lf);
		else if (id==1)
			UART_ByteSend(UART3, &lf);
	}

    if (id==0)
      UART_ByteSend(UART2, &c);
    else if (id==1)
      UART_ByteSend(UART3, &c);

    return true;
}

static cyg_bool 
str7xx_serial_getc_nonblock(serial_channel *chan, cyg_uint8* ch)
{
  str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
  cyg_uint8 id = str7xx_chan->id;
  u16 UARTStatus;

  if (id==0) {
    if(!(UART_FlagStatus(UART2) & UART_RxBufFull))
      return false; 
    UARTStatus = UART_ByteReceive(UART2, ch, 0xFF);
  }
  else if (id==1) {
    if(!(UART_FlagStatus(UART3) & UART_RxBufFull))
      return false; 
    UARTStatus = UART_ByteReceive(UART3, ch, 0xFF);
  }

  return true;
}

// Fetch a character from the device input buffer, waiting if necessary
static unsigned char 
str7xx_serial_getc(serial_channel *chan)
{
    cyg_uint8 c;

    while(!str7xx_serial_getc_nonblock(chan, &c));

    return c;
}

// Set up the device characteristics; baud rate, etc.
static Cyg_ErrNo
str7xx_serial_set_config(serial_channel *chan, cyg_uint32 key,
                         const void *xbuf, cyg_uint32 *len)
{
    switch (key) {
    case CYG_IO_SET_CONFIG_SERIAL_INFO:
      {
        cyg_serial_info_t *config = (cyg_serial_info_t *)xbuf;
        if ( *len < sizeof(cyg_serial_info_t) ) {
            return -EINVAL;
        }
        *len = sizeof(cyg_serial_info_t);
        if ( true != str7xx_serial_config_port(chan, config, false) )
            return -EINVAL;
      }
      break;
    default:
        return -EINVAL;
    }
    return ENOERR;
}

// Enable the transmitter on the device
static void
str7xx_serial_start_xmit(serial_channel *chan)
{
  str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
  cyg_uint8 id = str7xx_chan->id;

  (chan->callbacks->xmt_char)(chan);  // Kick transmitter (if necessary)
  if (id==0)
    UART_OnOffConfig(UART2, ENABLE);
  else if (id==1)
    UART_OnOffConfig(UART3, ENABLE);
}

// Disable the transmitter on the device
static void 
str7xx_serial_stop_xmit(serial_channel *chan)
{
    str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
    cyg_uint8 id = str7xx_chan->id;

    if (id==0)
      UART_OnOffConfig(UART2, DISABLE);
    else if (id==1)
      UART_OnOffConfig(UART3, DISABLE);
}

// Serial I/O - low level interrupt handler (ISR)
static cyg_uint32 
str7xx_serial_ISR(cyg_vector_t vector, cyg_addrword_t data)
{
    cyg_drv_interrupt_mask(vector);
    cyg_drv_interrupt_acknowledge(vector);
    return (CYG_ISR_CALL_DSR|CYG_ISR_HANDLED);  // Cause DSR to be run
}

// Serial I/O - high level interrupt handler (DSR)
static void       
str7xx_serial_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data)
{
    serial_channel *chan = (serial_channel *)data;
    str7xx_serial_info *str7xx_chan = (str7xx_serial_info *)chan->dev_priv;
    cyg_uint8 id = str7xx_chan->id;
    u8 c;

    // Only RX interrupts?
//    (chan->callbacks->xmt_char)(chan);

    if (id==0)
      UART_ByteReceive(UART2, &c, 0xFF);
    else if (id==1)
      UART_ByteReceive(UART3, &c, 0xFF);

    (chan->callbacks->rcv_char)(chan, c);

    cyg_drv_interrupt_unmask(vector);
}
#endif
