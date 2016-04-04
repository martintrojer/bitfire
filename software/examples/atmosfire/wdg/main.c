/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This program shows how to use the WDG software
*                      library.
********************************************************************************
* History:
* 13/01/2006   : V3.1
* 24/05/2005   : V3.0
* 30/11/2004   : V2.0
* 16/05/2003   : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"


//#define EXAMPLE_1      /*  Free running Timer Mode */
#define EXAMPLE_2        /*  WDG Mode */
#define Refresh_Time 1213


int main(void)
{
#ifdef EXAMPLE_2
 u32 counter=0xFFFFF;
 u8 count=0xFF;
#endif

#ifdef DEBUG
  debug();
#endif

#ifdef EXAMPLE_1
/*****************************************************************************
*Example 1: Free running Timer mode
******************************************************************************/
  /* GPIO peripheral configuration ------------------------------------------*/

  GPIO_Config (GPIO0, 0xF000, GPIO_OUT_PP);
  GPIO_WordWrite(GPIO0,0x0);
  
  /* WDG peripheral configuration -------------------------------------------*/
  /* Disable the end if count interrupt */  
  WDG_ECITConfig (DISABLE);
  /* Stop the free auto-reload timer to count down */
  WDG_CntOnOffConfig (DISABLE);
  /* Set the prescaler and counter reload value based on the needed time */
  WDG_PeriodValueConfig (12345);	/*  12.345 ms */
  
  /* EIC peripheral configuration -------------------------------------------*/
  /* The interrupt handler toggles an I/O port */  
  /* Configure the WDG_IRQChannel priority */   
  EIC_IRQChannelPriorityConfig(WDG_IRQChannel, 1);
  /* Enable the WDG_IRQChannel */  
  EIC_IRQChannelConfig(WDG_IRQChannel, ENABLE);
  /* Enable IRQ interrupts */
  EIC_IRQConfig(ENABLE);
  /* Enable the end of count interrupt */ 
  WDG_ECITConfig (ENABLE);
  /* The free auto-reload timer starts to count down */  
  WDG_CntOnOffConfig (ENABLE);
  while(1); /*  infinite loop */
#endif /* EXAMPLE_1 */


#ifdef EXAMPLE_2
/*****************************************************************************
*Example 2: Watchdog mode
******************************************************************************/
  
  /* GPIO peripheral configuration -------------------------------------------*/

  GPIO_Config (GPIO0, 0xFFFF, GPIO_OUT_PP);
  GPIO_WordWrite(GPIO0,0x0001);
 
  /* WDG peripheral configuration -------------------------------------------*/
  /* Set the counter prescaler value */
  WDG_PrescalerConfig (0xFF);
  /* Configure the counter pre-load value */  
  WDG_CntReloadUpdate (Refresh_Time);

  while(counter--);
  /* Enable the watchdog mode */
  WDG_Enable();

  while(1)
  {
    GPIO_WordWrite(GPIO0,0x0000);
    while(count--);
    count=0xFF;
    GPIO_WordWrite(GPIO0,0xFFFF);
    while(count--);
    count=0xFF;
    if(++counter==100)
    {
      counter=0;
    /* Refresh and update the watchdog counter in order to avoid a system reset */  
    WDG_CntRefresh();
    }
  }
#endif /* EXAMPLE_2 */
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/

