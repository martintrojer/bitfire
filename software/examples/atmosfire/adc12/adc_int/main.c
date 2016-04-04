/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file contains an ADC12 driver example using an
*                      interrupt service routine.
********************************************************************************
* History:
* 13/01/06 : V3.1
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
********************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "71x_lib.h"

int main (void)
{

 #ifdef DEBUG
  debug();
 #endif /* DEBUG */
  /* GPIO peripheral configuration ------------------------------------------*/

  /*  Configure the used analog input to HI_AIN_TRI */
  GPIO_Config (GPIO1, 0x0001, GPIO_HI_AIN_TRI);

  /*  Configure GPIO0 pins to output push-pull  */
  GPIO_Config (GPIO0, 0xFFFF, GPIO_OUT_PP);  

  /* EIC peripheral configuration -------------------------------------------*/

  /* Configure the EIC channel interrupt */
  EIC_IRQChannelPriorityConfig(ADC_IRQChannel,1);
  EIC_IRQChannelConfig(ADC_IRQChannel,ENABLE);
  EIC_IRQConfig(ENABLE);
  
  /* ADC12 peripheral configuration  ----------------------------------------*/

  /*  Initialize the conveter register. */
  ADC12_Init();

  /*  Configure the prescaler register using the configured PCLK with
   a sampling frequency=500Hz */
  ADC12_PrescalerConfig(500);

  /*  Select the conversion mode=single channel */
  ADC12_ModeConfig (ADC12_SINGLE);

  /*  Select the channel to be converted */
  ADC12_ChannelSelect(ADC12_CHANNEL0);

  /* Enable the ADC12 intruupts */
  ADC12_ITConfig (ENABLE);

  /*  Start the Converter */
  ADC12_ConversionStart();

  /*  Infinite loop */
  while(1);
}
