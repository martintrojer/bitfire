/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This program demonstrates how to use the UART with
*                      interrupt.
********************************************************************************
* History:
* 13/01/06 : V3.1
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

#define UART3_Rx_Pin (0x0001<<1)   /*  TQFP 64: pin N° 52 , TQFP 144 pin N° 123 */
#define UART3_Tx_Pin (0x0001<<0)   /*  TQFP 64: pin N° 53 , TQFP 144 pin N° 124 */

#define UART_Rx_Pin UART3_Rx_Pin
#define UART_Tx_Pin UART3_Tx_Pin



int main(void)
{
u16 i;
u8 bBuffer[4]={'S','T','R','7'};

#ifdef DEBUG
  debug();
  #endif

  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_RCLK_2);
  RCCU_PCLKConfig(RCCU_RCLK_2);
    
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);





  /* GPIO peripheral configuration -------------------------------------------*/

  /*  Configure the GPIO pins */
  GPIO_Config(GPIO0, UART_Tx_Pin, GPIO_AF_PP);
  GPIO_Config(GPIO0, UART_Rx_Pin, GPIO_IN_TRI_CMOS);

  /* UART peripheral configuration -------------------------------------------*/

  /*  Configure the UART X */
  /*  Turn UART0 on */
  UART_OnOffConfig(UART3, ENABLE); 
  /*  Disable FIFOs */      
  UART_FifoConfig (UART3, DISABLE);
  /*  Reset the UART_RxFIFO */      
  UART_FifoReset  (UART3 , UART_RxFIFO);
  /*  Reset the UART_TxFIFO */ 
  UART_FifoReset  (UART3 , UART_TxFIFO);
  /*  Disable Loop Back */ 
  UART_LoopBackConfig(UART3 , DISABLE);                                           
 /* Configure the UART0 as following:
                              - Baudrate = 9600 Bps
                              - No parity
                              - 8 data bits
                              - 1 stop bit */
  UART_Config(UART3,9600,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
  /*  Enable Rx */
  UART_RxConfig(UART3 ,ENABLE);
          
  /* EIC peripheral configuration --------------------------------------------*/

  /* Configure the EIC channel interrupt */
  EIC_IRQChannelPriorityConfig(UART3_IRQChannel, 1);
  EIC_IRQChannelConfig(UART3_IRQChannel, ENABLE);
  EIC_IRQConfig(ENABLE);

  UART_ItConfig(UART3,UART_RxBufFull, ENABLE);

  for(i=0;i<4;i++)
    {
      UART_ByteSend(UART3, (u8 *)&bBuffer[i]);
      /*  wait until the data transmission is finished */
      while(!((UART_FlagStatus(UART3)) & UART_TxEmpty)); 
    }

    while(1);

}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
