/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file gives an example on the use of the Output
*                      compare feature.
********************************************************************************
* History:
* 13/01/2006 : V3.1
* 24/05/05   : V3.0
* 30/11/04   : V2.0
* 16/05/03   : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"

/*
 This Device is configured to Toggle the TIM3 Output compare A pin from low
 level to high level after 0xF000 timer period. The GPIO pin is also toggled from
 low level to high level and may be used as a reference to verify the delay of the
 Output compare function.

                                    __________________________
                                    |                          TIM3 OCMPA
            ________________________|
                          ____________________________________
                         |                                     GPIO0 pin
            _____________|
                         <---------->
                         Output Compare Delay
*/

#define T3_OCMP_A 0x04

int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  /* ---------------------------------------------------------------------------
      Initialization
     --------------------------------------------------------------------------- */
  /*  Configure the GPIO Port */

  /*  Timer 3 Output Compare function pin (P1.2) configured as Alternate
      function pushpull */
  GPIO_Config (GPIO1, T3_OCMP_A, GPIO_AF_PP );

  /*  GPIO port configured as output push pull */
  GPIO_Config (GPIO0, 0xFFFF, GPIO_OUT_PP );

  /*  Initialise the GPIO0 port to low level */
  GPIO0->PD = 0x0000;

   /* ---------------------------------------------------------------------------
      Configure the EIC Timer3 IRQ channel
     --------------------------------------------------------------------------- */

  /*  Enable the IRQ0 for timer 3 */
  EIC_IRQChannelConfig( T3TIMI_IRQChannel, ENABLE );
  EIC_IRQChannelPriorityConfig( T3TIMI_IRQChannel, 1);
  EIC_IRQConfig( ENABLE );

  /* ---------------------------------------------------------------------------
      Configure the TIM3
     --------------------------------------------------------------------------- */
  /*  Initialize the Timer 3 registers to reset values */
  TIM_Init ( TIM3 );

  /*  Configure the TIM Prescaler */
  TIM_PrescalerConfig ( TIM3, 0x7F );

  /*  Enable the Output Compare for the TIM3 peripheral */
   TIM_ITConfig ( TIM3, TIM_OCA_IT, ENABLE );

  /*  Configure the TIM3 Output Compare Function */
     
  TIM_OCMPModeConfig ( TIM3, TIM_CHANNEL_A, 0xF000, TIM_WAVE, TIM_HIGH );
  /*                     |       |             |        |         |
                TIM 3 <--+       |             |        |         +--> Toggle To
                                 |             |        |              High Level
      TIM 3 Output compare       |             |        |
         channelA             <--+             |        +--> OCMP function used for
                                               |             external wave generation.
       Delay before toggling <-----------------+                                       */

  /* GPIO0 Forced To high Level */
  GPIO0->PD = 0xFFFF;

  /*  Infinite loop */
  while (1);
}
