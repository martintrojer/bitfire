/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file demonstrates how to use the BSPI software library
********************************************************************************
* History:
* 24/05/05 : V3.0
* 30/11/04 : V2.0
* 16/05/03 : Created
*******************************************************************************/

#include "71x_lib.h"

void do_test(void)
{
  u8 rt[10];
  int i;
  for (i=0;i<10;i++)
    rt[i]=0;
  
  while (BSPI_FlagStatus(BSPI1,BSPI_TFF)==SET)
    ;
  GPIO_BitWrite(GPIO1,8,0);             
  GPIO_BitWrite(GPIO0,12,1);                // When high FPGA ignores SPI traffic.
  BSPI_WordSend(BSPI1, 0x9f);
  BSPI_WordSend(BSPI1, 0x0);
  BSPI_WordSend(BSPI1, 0x0);
  BSPI_WordSend(BSPI1, 0x0);
  GPIO_BitWrite(GPIO0,12,0);
  GPIO_BitWrite(GPIO1,8,1);             

  BSPI_BufferReceive(BSPI1,rt,10);

  GPIO_BitWrite(GPIO2,13,0);    // write protect
  GPIO_BitWrite(GPIO2,13,1);             
}

int main (void)
{
  /*  Words to be transmitted */
  u32 tmp;

  #ifdef DEBUG
  debug();
  #endif

/* ---------------------------------------------------------------------------------------
   Communication between BSPI0(master) and BSPI1(slave)
      *BSPI0 send nine words of Sendtab to BSPI1 : BSPI0 ----- BSPI ----> BSPI1
      *BSPI1 receive nine words from BSPI0 in Receivetab : BSPI1 <---- BSPI ---- BSPI0
--------------------------------------------------------------------------------------- */

  /* Configure GPI01 on mode Alternate function Push Pull */
  GPIO_Config (GPIO0, 0x00FF, GPIO_AF_PP);
  
    // Setup clocks
  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_DEFAULT);
  RCCU_PCLKConfig(RCCU_RCLK_8);
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);
  //  RCCU_RCLKSourceConfig(RCCU_CLOCK2_16);
  
  //tmp = PCU->BOOTCR;

/* -------------------------------------------
Configure BSPI0 as a Master
------------------------------------------- */
  /* Enable the BSPI0 interface */
  //BSPI_FifoDisable(BSPI0);
  BSPI_Enable ( BSPI1 , ENABLE );

  /* Initialize BSPI0 */
  BSPI_Init ( BSPI1 ) ;

  /* Configure Baud rate Frequency: ---> APB1/6 */
  BSPI_ClockDividerConfig ( BSPI1, 8);

  /* Enable BSPI0 */
  BSPI_Enable ( BSPI1, ENABLE );

  /* Configure BSPI0 as a Master */
  BSPI_MasterEnable ( BSPI1,ENABLE );

  /* Configure the clock to be active high */
  BSPI_ClkActiveHigh(BSPI1,DISABLE);

  /* Enable capturing the first Data sample on the first edge of SCK */
  BSPI_ClkFEdge(BSPI1,DISABLE);

  /* Set the word length to 16 bit */
  BSPI_8bLEn(BSPI1,ENABLE);
  
  /*  Configure the depth of transmit to 9 words */
  BSPI_TrFifoDepth(BSPI1,10);
  BSPI_RcFifoDepth(BSPI1,10);
  /* Point on the word to transit */

//  GPIO_Config(GPIO0, 1<<3, GPIO_OUT_PP);

  GPIO_Config(GPIO0, 1<<12, GPIO_OUT_PP);   // Sync signal to FPGA. When high FPGA ignores SPI traffic.
  GPIO_Config(GPIO0, 1<<7, GPIO_OUT_PP);
  GPIO_Config(GPIO1, 1<<8, GPIO_OUT_PP);
  GPIO_Config(GPIO2, 1<<13, GPIO_OUT_PP);
  
  
  while(1)
    do_test();

}