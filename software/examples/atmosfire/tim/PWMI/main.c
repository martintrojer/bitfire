/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file gives an example of the use of the PWM input
*                      feature. The signal period is stored in the ICAR
*                      register and the pulse length in the ICBR register.
********************************************************************************
* History:
* 13/01/2006  : V3.1
* 24/05/2005  : V3.0
* 30/11/2004  : V2.0
* 16/05/2003  : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes-------------------------------------------------------------------*/
#include "71x_lib.h"
/* Include of other module interface headers ---------------------------------*/
#include <stdio.h> 

#define T3_ICAP_A 0x02

  u32 Fpclk2 ;
  u16 Tmp1 = 0x0;
  u16 Tmp2 = 0x0;
  u16 Prescaler;
  u32 Frequency;
  u32 DutyCycle;

int main(void)
{
  #ifdef DEBUG
  debug ();
  #endif

  /* ---------------------------------------------------------------------------
      Using an external 16 MHz to generate a 32 MHz frequency 
    --------------------------------------------------------------------------- */


  /*  Configure the PLL1 ( * 12 , / 3 ) */
  RCCU_PLL1Config (RCCU_PLL1_Mul_12, RCCU_Div_3) ;

  /*  Wait PLL to lock */
  while(RCCU_FlagStatus(RCCU_PLL1_LOCK)==RESET);

  /*  Select PLL1_Output as RCLK clock */
  RCCU_RCLKSourceConfig (RCCU_PLL1_Output);


  /* ---------------------------------------------------------------------------
      Initialization
     --------------------------------------------------------------------------- */

  /*  Configure the GPIO1 Port */
  /*  Timer 3 Input Capture A function pin (P1.1) configured as Input
      Tristate TTL. */
  GPIO_Config (GPIO1, T3_ICAP_A, GPIO_IN_TRI_TTL );

  /*  Initialize the Timer */
  TIM_Init ( TIM3 );

  /* ---------------------------------------------------------------------------
      Configure the TIM3
     --------------------------------------------------------------------------- */

  /*  Configure the prescaler */
  TIM_PrescalerConfig ( TIM3, 0x00 );
 
  /*  Configure the PWM input Mode */
  TIM_PWMIModeConfig ( TIM3, TIM_RISING );

  /*  Start the Counter */
  TIM_CounterConfig ( TIM3, TIM_START );

  /*  Get the APB2 Frequency */
  Fpclk2 = RCCU_FrequencyValue(RCCU_PCLK);

  while(1)
   {
      /* Wait until ICFB flag is set  */
      while (TIM_FlagStatus(TIM3,TIM_ICFB) == RESET);
      Tmp1 = TIM3->ICBR;

      /* Wait until ICFA flag is set  */
      while (TIM_FlagStatus(TIM3,TIM_ICFA) == RESET);
      Tmp2 = TIM3->ICAR;    

      /*  Compute the input signal Frequency and DutyCycle */ 
      Frequency = Fpclk2/( 0x1 *Tmp2 );
      DutyCycle =100 * Tmp1/Tmp2;

      /*  Clear TIMER Flags */ 
      TIM_FlagClear(TIM3,TIM_ICFA);
      TIM_FlagClear(TIM3,TIM_ICFB);

      /*  Display on the debugger output window the Frequency and 
          the DutyCycle of the input signal */ 
      printf("Frequency = %dHz DutyCycle = %2d%%\n",Frequency,DutyCycle);
   }

}
