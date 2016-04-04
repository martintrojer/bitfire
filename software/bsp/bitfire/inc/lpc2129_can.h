// LPC2129 CAN
// mtrojer@arrownordic.com
// $WCREV$
// Common include file for CAN drivers.
//! Common include file for CAN drivers.
//! Defines CAN_* constants.
//! Defines struct can_control
//! \page LPC2129_CAN lpc2129_can.h 
//! \ingroup BSPINC

#ifndef _LPC2129_CAN_H
#define _LPC2129_CAN_H

#include "bitfire_types.h"

typedef struct {
  UNS_8 port;
  UNS_16 id;
  UNS_32 baudrate;
  UNS_8 status;
} can_control;

#define CAN_STATUS_NOT_STARTED 0x00
#define CAN_STATUS_OK          0xFF

#endif

