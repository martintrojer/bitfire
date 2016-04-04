// UART INT
// mtrojer@arrownordic.com
// $WCREV$
//! Interrupt-driven UART driver
//! \page LPC2129_UART_INT lpc2129_uart_int.h 
//! \ingroup BSPINC

#ifndef _LPC2129_UART_INT_H
#define _LPC2129_UART_INT_H

#include "lpc2129_uart.h"

//! \defgroup lpc2129_uart_int lpc2129_uart_int

//! Initializes the UART peripheral
//! @param *uc Pointer to initialized uart_control struct.
//! Will update the status field.
//!
//! Example usage:
//! <pre>
//!   uart_control uart0; \n
//!   uart0.port=0;       \n                
//!   uart0.LCR = LCR_WLS_8BIT | LCR_SBS_1BIT | LCR_PE_0 | LCR_PS_ODD | LCR_BK_0 | LCR_DLAB_1; \n
//!   uart0.baudrate = 115200;   \n
//!   uart0.vecaddr=2;           \n
//! \n
//!   uart_int_init(&uart0);     \n
//! </pre>
//! \ingroup lpc2129_uart_int
void  uart_int_init(uart_control *uc);

//! Writes 8 bits to the UART peripheral
//! @param *uc Pointer to initialized uart_control struct.
//! You must call uart_int_init() first.
//! @param ch Containing the 8 bits to be sent.
//! \ingroup lpc2129_uart_int
UNS_8 uart_int_putchar(uart_control *uc, UNS_8 ch);

//! Reads 8 bits from the UART periferal. This is non-blocking call.
//! @param *uc Pointer to initialized uart_control struct.
//! @param *err Pointer to a 8bit charcter that will hold the error
//! code. It will be one of the following:
//! UART_ERR_NO_ERROR, UART_ERR_NOT_INIT, UART_ERR_NO_CHAR_READY
//! @return Read character 
//! \ingroup lpc2129_uart_int
UNS_8 uart_int_getchar(uart_control *uc, UNS_8 *err);

#endif
