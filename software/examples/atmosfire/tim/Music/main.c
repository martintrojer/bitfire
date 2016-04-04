/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file gives an example on the PWM generation.
*                      The device is configured to generate a PWM wave with a
*                      pulse equal to 0x30 timer clock ticks and a full period
*                      equal to 0xC0.
********************************************************************************
* History:
* 13/01/2006 : V3.1
* 24/05/2005 : V3.0
* 30/11/2004 : V2.0
* 16/05/2003 : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#include "71x_lib.h"
#define T1_OCMP_A 1<<7

#define _SI0 0x1F10
#define _DO  0x1DD1
#define _RE  0x1A9E
#define _MI  0x179A
#define _FA  0x164A
#define _SOL 0x13DA
#define _LA  0x11CB
#define _SI  0x0F90
#define _DO2 0x0EE8
#define _M   0x0088

#define min 0x001
#define sq  0x010
#define q   0x020
#define qp  0x040
#define c   0x080
#define cp  0x100
#define m   0x200



 u32 Tab1[30 ] = { _M , m,_DO, cp,_M , min,_DO, cp ,_M , min,
                   _DO, cp,_RE, cp,_MI, m ,_RE, m ,
                   _DO, cp,_MI, cp,_RE, cp ,_M , min,_RE, cp ,
                   _DO, m
                 };

/*
 u32 Tab1[56 ] = { _M , m,_DO, c ,_RE, c,_MI, c,_DO, c,
                   _RE, m,_M , sq,_RE, c,_MI, c,_FA, c,
                   _M ,sq,_FA, cp,_MI, c,_M ,sq,_MI,cp,
                   _M , c,_DO, c ,_RE, c,_MI, c,_DO, c,
                   _RE, m,_M , sq,_RE, c,_MI, c,_FA, m,
                   _SOL,m,_DO, m ,_DO, m };
*/

u32  NoteIndex = 0x00;
u32  Note, Volume, Period;


void PlayNextTone(void)
{
  if ( Period > 0 )
  {
    Period --;
  }
  else
  {
    if ( NoteIndex < sizeof(Tab1)/sizeof(Tab1[0]) )
    {
      Note   = Tab1[ NoteIndex   ];
      Period = Tab1[ NoteIndex+1 ];
      Volume = Note * 0.5;
      if ( Note == 0x88 ) { Volume = 0x89; }
      TIM_PWMOModeConfig ( TIM1, Volume, TIM_HIGH, Note, TIM_LOW );
      NoteIndex += 2;
    }
    else
    {
      NoteIndex  = 0x0;
      Period     = 0x0;
    }
  }
}


int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  /* configure pin wakeup button as input TTL */
  GPIO_Config(GPIO0,0x8000,GPIO_IN_TRI_TTL);

  /* configure P0.0 as output push-pull for LED */
  GPIO_Config(GPIO0,0x0001,GPIO_OUT_PP);
  GPIO0->PD=0;

  /* Initialize the XTI*/
  XTI_Init();

  /* Configure the Line 2 mode, select Falling edge*/
  XTI_LineModeConfig(XTI_Line15, XTI_FallingEdge);

  /* Enable line 15 */
  XTI_LineConfig(XTI_Line15, ENABLE);
   
  /* Enable Wake-Up mode in the XTI */
  XTI_ModeConfig(XTI_WakeUpInterrupt, ENABLE);

  /*  Variable Initialization */
  Note   = 0;
  Period = 0;
  Volume = 0;
  if ( Note == 0xFFFB ) ( Volume = 3 );


  /* ---------------------------------------------------------------------------
     Configure the EIC Timer1 IRQ channel
     --------------------------------------------------------------------------- */

  /*  Enable the IRQ0 for timer 1 */
  EIC_IRQChannelConfig( T1TIMI_IRQChannel, ENABLE );
  EIC_IRQChannelPriorityConfig( T1TIMI_IRQChannel , 1);
  EIC_IRQConfig( ENABLE );

  /*  Enable the IRQ for the XTI */
  EIC_IRQChannelConfig( XTI_IRQChannel, ENABLE );
  EIC_IRQChannelPriorityConfig( XTI_IRQChannel , 2);
  EIC_IRQConfig( ENABLE );

  /*---------------------------------------------------------------------------
    Initialization
    --------------------------------------------------------------------------- */
  /*  Configure the GPIO1 Port */
  /*  Timer 1 Output Compare function pin (P1.7) configured as Alternate function pushpull */
  GPIO_Config ( GPIO1, T1_OCMP_A, GPIO_AF_PP );

  /*  Initialize the Timer */
  TIM_Init ( TIM1 );

  /*  Configure the TIM Prescaler */
  TIM_PrescalerConfig ( TIM1, 0x01 );

  /*  Enable the OverFlow Interrupt */
  TIM_ITConfig ( TIM1 , TIM_TO_IT , ENABLE );
  /*  Start Timer1 */
  TIM_CounterConfig ( TIM1, TIM_START );

  while (1);
}



