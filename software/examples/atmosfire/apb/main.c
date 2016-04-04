/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file is used to demonstrate the APB software library.
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

  /*  Disable the I2C0 & I2C1 clocks */
  APB_ClockConfig( APB1, DISABLE, I2C0_Periph | I2C1_Periph ) ;

  /*  Keep all I2C0 and I2C1 under Reset */
  APB_SwResetConfig( APB1, ENABLE, I2C0_Periph | I2C1_Periph) ;

  /*  Enable clocks for UART 0,1,2 &3 */
  APB_ClockConfig( APB1, ENABLE,  UART0_Periph | UART1_Periph\
                                | UART2_Periph | UART3_Periph ) ;

  /*  Disable Reset for UART 0,1,2 &3 */
  APB_SwResetConfig( APB1, DISABLE, UART0_Periph | UART1_Periph\
                                  | UART2_Periph | UART3_Periph ) ;

  while(1);
}
