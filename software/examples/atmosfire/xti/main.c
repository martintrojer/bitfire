/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This program demonstrates how to use the XTI software
*                      library.
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

int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  /* XTI peripheral configuration ------------------------------------------*/

  /* Initialize the XTI */
  XTI_Init();

  /* Set Line 15 edge */
  XTI_LineModeConfig(XTI_Line15, XTI_FallingEdge);

  /* Enable the External interrupts on line 15 */
  XTI_LineConfig(XTI_Line15, ENABLE);

  /* Set the XTI mode */
  XTI_ModeConfig(XTI_Interrupt, ENABLE);

  /* Configure the XTI IRQ channel -----------------------------------------*/

  /*  Set the XTI IRQ Channel priority to 1*/
  EIC_IRQChannelPriorityConfig(XTI_IRQChannel,1);

  /* Enable XTI IRQ Interrupts */
  EIC_IRQChannelConfig(XTI_IRQChannel,ENABLE);

  /* Enable IRQ interrupts on EIC */
  EIC_IRQConfig(ENABLE);

  /* GPIO peripheral configuration ------------------------------------------*/

  /* Configure the P0.15 in IPUPD WP mode */
  GPIO_Config(GPIO0,0x0001<<0x0F,GPIO_IPUPD_WP);

  /* Configure the P0.0 in Push-Pull Output mode */
  GPIO_Config(GPIO0,0x0001,GPIO_OUT_PP);

  while(1); /*  infinite loop */
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
