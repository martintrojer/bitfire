/******************** (C) COPYRIGHT 2003 ST Microelectronics *******************
* File Name          : main.h
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
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

#ifndef __main_H
#define __main_H

// used message object numbers
enum {
	CAN_TX_MSGOBJ = 0,
	CAN_RX_MSGOBJ = 1
};

// LED function prototypes
extern void LED_On(u32 number);
extern void LED_Off(u32 number);
extern void LED_Toggle(u32 number);

#endif // __main_H

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
