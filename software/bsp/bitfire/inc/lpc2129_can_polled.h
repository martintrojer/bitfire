// CAN polled
// mtrojer@arrownordic.com
// $WCREV$
//! Polled CAN driver
//! \page LPC2129_CAN_POLLED lpc2129_can_polled.h 
//! \ingroup BSPINC

#ifndef _LPC2129_CAN_POLLED_H
#define _LPC2129_CAN_POLLED_H

#include "lpc2129_can.h"

//! \defgroup lpc2129_can_polled lpc2129_can_polled

//! Initializes the CAN peripheral
//! @param *cc Pointer to initialized can_control struct.
//! Will update the status field.
//!
//! Example usage:
//! <pre>
//!   can_control tx = {1,0x55,20000000,0};  \n
//!   can_polled_init(&tx);  \n
//! </pre>
//! \ingroup lpc2129_can_polled
void  can_polled_init(can_control *cc);

//! Writes 1-8 8bit words to the CAN peripheral
//! @param *cc Pointer to initialized can_control struct.
//! You must call can_polled_init() first.
//! @param dlc Counter-value of how many 8bit words to send.
//! @param *data Pointer to 8bit databuffer with data to send.
//! \ingroup lpc2129_can_polled
UNS_8 can_polled_send(can_control *cc, UNS_8 dlc, UNS_8 *data);

//! Reads 1-8 8bit words from the CAN peripheral. Blocks until data is ready.
//! @param *cc Pointer to initialized can_control struct.
//! You must call can_polled_init() first.
//! @param *data Pointer to 8bit databuffer of max length 8.
//! \ingroup lpc2129_can_polled
UNS_8 can_polled_receive(can_control *cc, UNS_8 *data);

//! Clears the RX buffer in the CAN peripheral
//! @param *cc Pointer to initialized can_control struct.
//! You must call can_polled_init() first.
//! \ingroup lpc2129_can_polled
void can_polled_clear_rxbuf(can_control *cc);

//! Set acceptance filters in the CAN peripheral
//! @param *filters Pointer to a filter struct.
//! Please note that this function is not yet implemented!
//! If sets the CAN periferal in bypass mode (no filter mode).
//! \ingroup lpc2129_can_polled
void can_polled_set_filters(void *filters);

#endif
