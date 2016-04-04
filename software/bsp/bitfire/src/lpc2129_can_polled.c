// CAN polled
// mtrojer@arrownordic.com
// $WCREV$

#include "bitfire_types.h"
#include "bitfire.h"
#include "lpc21xx.h"
#include "lpc2129_can.h"
#include "lpc2129_can_polled.h"

void can_polled_init(can_control *cc)
{
  UNS_32 bauddivisor = VPB_CLOCK / (16*(cc->baudrate));
 
  switch(cc->port)
    {
    case 1:
      PINSEL1 &= ~(1<<19);
      PINSEL1 |= (1<<18);

      C1MOD |= 1<<0;    // set RM
      C1MOD &= 1<<0;    // clear other bits
      C1BTR = (C1BTR & ~(0x1ff)) + bauddivisor;
      C1TID1 = cc->id;
      C1MOD &= ~(1<<0); // reset RM

      cc->status = CAN_STATUS_OK;
      break;
    case 2:
      PINSEL1 &= ~(1<<15 | 1<<17);
      PINSEL1 |= (1<<14 | 1<<16);

      C2MOD |= 1<<0;    // set RM
      C2MOD &= 1<<0;    // clear other bits
      C2BTR = (C2BTR & ~(0x1ff)) + bauddivisor;
      C2TID1 = cc->id;
      C2MOD &= ~(1<<0); // reset RM

      cc->status = CAN_STATUS_OK;
      break;
    default:
      cc->status = CAN_STATUS_NOT_STARTED;
      break;
    }
}

UNS_8 can_polled_send(can_control *cc, UNS_8 dlc, UNS_8 *data)
{
  UNS_8 i,mydlc=dlc;
  
  if (cc->status!=CAN_STATUS_OK)
    return 0;

  if (mydlc>8) mydlc=8;

  switch (cc->port)
    {
    case 1:
      C1TFI1 = (C1TFI1 & (~0xF0000))+(mydlc<<16);
      C1TDA1 = 0;
      C1TDB1 = 0;
      for (i=0;i<mydlc;i++) {
	if (i>3)
	  C1TDB1 |= *(data+i)<<(8*(i%4));
	else 
	  C1TDA1 |= *(data+i)<<(8*(i%4));	  
      }
      C1CMR |= (1<<0);
      return C1GSR;
    case 2:
      C2TFI1 = (C2TFI1 & (~0xF0000))+(mydlc<<16);
      C2TDA1 = 0;
      C2TDB1 = 0;
      for (i=0;i<mydlc;i++) {
	if (i>3)
	  C2TDB1 |= *(data+i)<<(8*(i%4));
	else
	  C2TDA1 |= *(data+i)<<(8*(i%4));
      }
      C2CMR |= (1<<0);
      return C2GSR;
    default:
      break;
    }
}

UNS_8 can_polled_receive(can_control *cc, UNS_8 *data)
{
  UNS_8 i, mydlc;

  if (cc->status!=CAN_STATUS_OK) 
    return 0;

  switch (cc->port)
    {
    case 1:
      while(!(C1GSR & (1<<0)))
	;
      mydlc = (C1RFS>>16) & 0xf;
      if (mydlc>8) mydlc=8;
      for (i=0;i<mydlc;i++) {
	if (i>3)
	  data[i] = C1RDB>>(8*(i%4));
	else
	  data[i] = C1RDA>>(8*(i%4));
      }
      return C1GSR;
    case 2:
      while(!(C2GSR & (1<<0)))
	;
      mydlc = (C2RFS>>16) & 0xf;
      if (mydlc>8) mydlc=8;
      for (i=0;i<mydlc;i++) {
	if (i>3)
	  data[i] = C2RDB>>(8*(i%4));
	else
	  data[i] = C2RDA>>(8*(i%4));
      }
      return C2GSR;
    default:
      break;
    }
}

void can_polled_clear_rxbuf(can_control *cc)
{
  if (cc->status!=CAN_STATUS_OK) 
    return;

  switch (cc->port)
    {
    case 1:
      C1CMR |= (1<<2);
      break;
    case 2:
      C2CMR |= (1<<2);
      break;
    default:
      break;
    }
}

void can_polled_set_filters(void *filters)
{
  AFMR |= (1<<1);		// Enable bypass 
}
