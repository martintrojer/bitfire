/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This program demonstrates how to use the UART with the
*                      STR71x software library.
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
#include "rccu.h"

#define UART2_Rx_Pin (0x0001<<13)  /*  TQFP 64: pin N° 5  , TQFP 144 pin N° 9 */
#define UART2_Tx_Pin (0x0001<<14)  /*  TQFP 64: pin N° 6  , TQFP 144 pin N° 10 */

#define UART3_Rx_Pin (0x0001<<1)   /*  TQFP 64: pin N° 52 , TQFP 144 pin N° 123 */
#define UART3_Tx_Pin (0x0001<<0)   /*  TQFP 64: pin N° 53 , TQFP 144 pin N° 124 */

#define  UARTX  UART3
#define  UARTX_Rx_Pin  UART3_Rx_Pin
#define  UARTX_Tx_Pin  UART3_Tx_Pin
#define  UARTX_Periph  UART3_Periph

u16 i;
u16 UARTStatus;
u8 bBuffer[4]={'S','T','R','7'};

int kalle (void) __attribute__ ((section ("bss")));

int kalle(void)
{
}

int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_DEFAULT);
  RCCU_PCLKConfig(RCCU_RCLK_2);
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);

   /* GPIO peripheral configuration -------------------------------------------*/

  /*  Configure the GPIO pins */
  GPIO_Config(GPIO0, UARTX_Tx_Pin, GPIO_AF_PP);
  GPIO_Config(GPIO0, UARTX_Rx_Pin, GPIO_IN_TRI_CMOS);
  
  /* UART peripheral configuration -------------------------------------------*/

  /*  Configure the UART X */
  UART_Init(UARTX);
  /*  Turn UARTX on */
  UART_OnOffConfig(UARTX, ENABLE); 
  /*  Disable FIFOs */      
  UART_FifoConfig (UARTX, DISABLE); 
  /*  Reset the UART_RxFIFO */     
  UART_FifoReset  (UARTX , UART_RxFIFO); 
  /*  Reset the UART_TxFIFO */
  UART_FifoReset  (UARTX , UART_TxFIFO); 
  /*  Disable Loop Back */
  UART_LoopBackConfig(UARTX , DISABLE);                                           
 /* Configure the UARTX as following:
                             - Baudrate = 9600 Bps
                             - No parity
                             - 8 data bits
                             - 1 stop bit */
  UART_Config(UARTX,38400,UART_NO_PARITY,UART_1_StopBits,UARTM_8D);
 /*  Enable Rx */
  UART_RxConfig(UARTX ,ENABLE);          
  while(1)
  {
    for(i=0;i<4;i++)
    {
      UART_ByteSend(UARTX, (u8 *)&bBuffer[i]);
      /*  wait until the data transmission is finished */
      while(!((UART_FlagStatus(UARTX)) & UART_TxEmpty)); 
    }

    for(i=0;i<4;i++)
    {
      /*  wait until data is received */       
      while(!(UART_FlagStatus(UARTX) & UART_RxBufFull));  
      /*  Get the received data, set the guard time to 0xFF */
      UARTStatus = UART_ByteReceive(UARTX, (u8 *)&bBuffer[i], 0xFF); 
    }
   }
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
