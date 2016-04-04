/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : EMI library example
*                      This program is loaded then executed from the external
*                      memory, thanks to the 71x_init.s start-up file.
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

void delay(void)
{
  u16 i;
  for(i=0; i<0xFFFF; i++); /* delay */
}

int main(void)
{
  #ifdef DEBUG
  debug();
  #endif
  /* GPIO peripheral configuration ------------------------------------------*/

  /*  Configure GPIO0 as PP_OUT */
  GPIO_Config(GPIO0,0xFFFF,GPIO_OUT_PP);

  /*  Set all GPIO0 pins low */
  GPIO_WordWrite(GPIO0,0);

  while(1) /*  infinite loop */
  {
    delay();
    /*  Toggle GPIO0 pins */
    GPIO_WordWrite(GPIO0,~GPIO_WordRead(GPIO0));
  }
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
