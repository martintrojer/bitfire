/******************** (C) COPYRIGHT 2003 ST Microelectronics *******************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This program demonstrates a basic example of how to use
*                      the CAN software library.
*                      It uses the Loopback mode combined with Silent mode
*                      (self-test) so it does not need a true CAN bus to work.
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"

#include "main.h"

//u32 Image$$ZI$$Limit = 0x1000;	// debug

/*****************************************************************************/

// buffer for receive messages
canmsg RxCanMsg;

// array of pre-defined transmit messages
canmsg TxCanMsg[2] = {
	{ CAN_STD_ID,      0x123, 4, { 0x01, 0x02, 0x04, 0x08 } },
	{ CAN_EXT_ID, 0x12345678, 8, { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 } }
};

/*****************************************************************************/

// I/O mapping of the 16 LEDs
struct {
	GPIO_TypeDef** gpio;
	u32 port;
} LED_Map[16] = {
	{ &GPIO0,  3 },
	{ &GPIO0,  2 },
	{ &GPIO0,  1 },
	{ &GPIO0,  0 },
	{ &GPIO0, 12 },
	{ &GPIO1, 15 },
	{ &GPIO2,  9 },
	{ &GPIO2, 10 },
	{ &GPIO2, 11 },
	{ &GPIO2, 12 },
	{ &GPIO2, 13 },
	{ &GPIO2, 14 },
	{ &GPIO2, 15 },
	{ &GPIO1,  6 },
	{ &GPIO1,  5 },
	{ &GPIO1,  4 }
};

void LED_Init(void)
{
	u32 i;
	unsigned short uMask;

	for (i=0; i<16; i++)
	{
		// setup the I/O port as output / Push-pull
		uMask = 1 << LED_Map[i].port;
		(*LED_Map[i].gpio)->PC0 |= uMask;
		(*LED_Map[i].gpio)->PC1 &=~uMask;
		(*LED_Map[i].gpio)->PC2 |= uMask;

		(*LED_Map[i].gpio)->PD &=~uMask;
	}
}

void LED_On(u32 number)
{
	(*LED_Map[number].gpio)->PD |= 1 << LED_Map[number].port;
}

void LED_Off(u32 number)
{
	(*LED_Map[number].gpio)->PD &= ~(1 << LED_Map[number].port);
}

void LED_Toggle(u32 number)
{
	(*LED_Map[number].gpio)->PD ^= 1 << LED_Map[number].port;
}

/*****************************************************************************/

void IO_Init(void)
{
	u16 uMask;

	// setup GPIO 1.11 (CAN RX pin) as input Tristate CMOS
	uMask = 1<<11;
	GPIO1->PC0 &=~uMask;
	GPIO1->PC1 |= uMask;
	GPIO1->PC2 &=~uMask;

	// setup GPIO 1.12 (CAN TX pin) as output alternate Push-pull
	uMask = 1<<12;
	GPIO1->PC0 |= uMask;
	GPIO1->PC1 |= uMask;
 	GPIO1->PC2 |= uMask;
}


void wait(void)
{
	u32 i;
	for (i=0; i<100000L; i++) {}
}


void Demo_Polling(void)
{
	// initialize the CAN at a standard bitrate, interrupts disabled
	CAN_Init(0, CAN_BITRATE_100K);

	// switch into Loopback+Silent mode (self-test)
	CAN_EnterTestMode(CAN_TESTR_LBACK | CAN_TESTR_SILENT);
	//	CAN_EnterTestMode(CAN_TESTR_BASIC);
	CAN_LeaveTestMode();
	
	// configure the message objects
	CAN_InvalidateAllMsgObj();
	CAN_SetTxMsgObj(CAN_TX_MSGOBJ, CAN_STD_ID);
	CAN_SetRxMsgObj(CAN_RX_MSGOBJ, CAN_STD_ID, 0, CAN_LAST_STD_ID, TRUE);

	//	LED_On(0);

	// send a pre-defined data frame
	(void)CAN_SendMessage(CAN_TX_MSGOBJ, &TxCanMsg[0]);

	// wait until end of transmission
	CAN_WaitEndOfTx();

	//	LED_On(1);

	// wait for reception of a data frame
	while (!CAN_ReceiveMessage(CAN_RX_MSGOBJ, FALSE, &RxCanMsg))
	{
		// add a time-out handling here, if necessary
	}

	//	LED_On(2);

	// release the message objects
	CAN_ReleaseTxMessage(CAN_TX_MSGOBJ);
	CAN_ReleaseRxMessage(CAN_RX_MSGOBJ);

	//	LED_On(3);

	// switch back into Normal mode
	CAN_LeaveTestMode();
}


void Demo_Interrupt(void)
{
	// initialize the interrupt controller
	EIC_IRQChannelConfig(CAN_IRQChannel, ENABLE);
	EIC_IRQChannelPriorityConfig(CAN_IRQChannel, 1);
	EIC_IRQConfig(ENABLE);

	// initialize the CAN at a standard bitrate, interrupts enabled
	CAN_Init(CAN_CR_IE, CAN_BITRATE_500K);

	// switch into Loopback+Silent mode (self-test)
	CAN_EnterTestMode(CAN_TESTR_LBACK | CAN_TESTR_SILENT);

	// configure the message objects
	CAN_InvalidateAllMsgObj();
	CAN_SetTxMsgObj(CAN_TX_MSGOBJ, CAN_EXT_ID);
	CAN_SetRxMsgObj(CAN_RX_MSGOBJ, CAN_EXT_ID, 0, CAN_LAST_EXT_ID, TRUE);

	LED_On(4);

	// send a pre-defined data frame
	(void)CAN_SendMessage(CAN_TX_MSGOBJ, &TxCanMsg[1]);

	// reception and release are done in the interrupt handler

	wait();

	LED_On(7);

	// switch back into Normal mode
	CAN_LeaveTestMode();

	// disable interrupts globally
	EIC_IRQConfig(DISABLE);
}

/*****************************************************************************/

u32 main(void)
{
	#ifdef DEBUG
	debug();
	#endif	

	// initialize the I/O ports for the CAN pins
	IO_Init();
	while(1)
	  Demo_Polling();


	
	// initialize the I/O ports for the LEDs
	LED_Init();

	Demo_Polling();

	Demo_Interrupt();

	// if the program has run properly, 8 LEDs must be lit

	return 0;
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
