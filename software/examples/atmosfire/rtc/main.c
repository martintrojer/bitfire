/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : Example of using the RTC software library
********************************************************************************
* History:
* 13/01/05 : V3.1
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

#define _GPIO
#define _GPIO1

#include "71x_lib.h"

/* Define the RTC prescaler 0x8000 to have 1 second basic clock */
#define RTC_Prescaler 32768 

int main (void)
{
  #ifdef DEBUG
    debug();
  #endif

/*  Configure RTC prescaler */
    RTC_PrescalerConfig ( RTC_Prescaler );

/*  Clear Pending Flags */
    RTC_FlagClear ( RTC_OWIR );
    RTC_FlagClear ( RTC_AIR  );
    RTC_FlagClear ( RTC_SIR  );
    RTC_FlagClear ( RTC_GIR  );

 #if 0
    {
    unsigned int val;
    while(1) {
      val =  RTC_CounterValue();
    }
  }  
#endif
    
/*  Enable RTC IRQ channel */
  EIC_IRQChannelConfig( RTC_IRQChannel, ENABLE );
  EIC_IRQChannelPriorityConfig( RTC_IRQChannel, 1);
  EIC_IRQConfig( ENABLE );

/*  Enable Second Interrupt */
  RTC_ITConfig( RTC_SIT, ENABLE );
  RTC_ITConfig( RTC_GIT, ENABLE );

/*  Configure Port 0 pins */
  GPIO_Config (GPIO1, 1<<9, GPIO_OUT_PP);
  GPIO_BitWrite(GPIO1, 9, 1);

/*  never-ending loop */
  while (1);
}
