/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file gives an example of PWM generation.
********************************************************************************
* History:
* 13/01/2006 : V3.1
* 24/05/2005 : V3.0
* 30/11/2004 : V2.0
* 16/05/2003 : Created
********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"

#define T3_OCMP_A 0x04

int main(void)
{
  #ifdef DEBUG
  debug ();
  #endif

  /* ---------------------------------------------------------------------------
      Initialization
     --------------------------------------------------------------------------- */
  /*  Configure the GPIO1 Port */
  /*  Timer 3 Output Compare function pin (P1.2) configured as Alternate
      function pushpull. */
  GPIO_Config (GPIO1 , T3_OCMP_A , GPIO_AF_PP );

  /*  Initialize the Timer */
  TIM_Init ( TIM3 );

  /*  Configure the TIM Prescaler */
  TIM_PrescalerConfig ( TIM3, 0x1 );

  /*  Generate a PWM Signal :
       - Full Period  = 7FFF
       - Duty Cycle   = 50 % ( 3FFF ) */
  TIM_PWMOModeConfig ( TIM3, 0x3FFF, TIM_HIGH, 0x7FFF, TIM_LOW );

  /*  Start The Counter */
  TIM_CounterConfig ( TIM3, TIM_START );

  /*  Infinite loop */
  while (1);
}
