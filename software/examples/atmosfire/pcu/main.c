/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 20/06/2003
* Description        : This file contains a basic example to use the PCU
*                      software library.
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
  u32 i,j;
  #ifdef DEBUG
  debug();
  #endif
  
  /* configure pin wakeup button as input TTL */
  GPIO_Config(GPIO0,0x8000,GPIO_IN_TRI_TTL);
  /* configure P0.0 as output push-pull for LED */
  GPIO_Config(GPIO0,0x0001,GPIO_OUT_PP);
  GPIO0->PD=0;

  /* uncomment the following ligne to enable DEBUG in STOP mode */
  //    PCU->BOOTCR|=1<<4;


  /* Initialize the XTI*/
  XTI_Init();

  /* Configure the Line 2 mode, select Falling edge*/
  XTI_LineModeConfig(XTI_Line15, XTI_FallingEdge);

  /* Enable line 15 */
  XTI_LineConfig(XTI_Line15, ENABLE);
   
  /* Enable Wake-Up mode in the XTI */
  XTI_ModeConfig(XTI_WakeUp, ENABLE);
  
 
  while(1)
 { 
  for(i=0;i<10;i++)
  {
    for(j=0;j<0xFFFF;j++);
    GPIO_BitWrite(GPIO0,0,~GPIO_BitRead(GPIO0,0));
  }

  GPIO_BitWrite(GPIO0,0,0);

  /* call stop mode function */
  PCU_LPMEnter ( PCU_STOP );

  /* Exit From STOP mode is done by the a falling edge on the P0.15 pad.*/

  XTI_PendingBitClear(XTI_InterruptLineValue());
 }

}

