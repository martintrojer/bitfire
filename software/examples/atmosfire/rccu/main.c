 /******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file presents a basic example to use the RCCU
*                      software library
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

  /* RCCU peripheral configuration ------------------------------------------*/
   
  /*  Configure FCLK = RCLK /2 */
  RCCU_FCLKConfig ( RCCU_RCLK_2 );

  /*  Configure PCLK = RCLK /2 */
  RCCU_PCLKConfig (RCCU_RCLK_2);

  /*  Configure MCLK clock for the CPU, RCCU_DEFAULT = RCLK /1 */
  RCCU_MCLKConfig (RCCU_DEFAULT);
   
  /*  Configure the PLL1 ( * 12 , / 4 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_4) ;

  /*  Wait PLL to lock */
  while(RCCU_FlagStatus(RCCU_PLL1_LOCK)==RESET);

  /*  Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output) ;

  /*  At this step the CKOUT signal should be equal to 12 MHz with
     an external oscilator equal to 16MHz */
  
  while(1);
}