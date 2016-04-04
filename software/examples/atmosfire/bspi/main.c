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

int main (void)
{
  /*  Words to be transmitted */

  u8 Sendtab[9]={'S','T','R','7','_','B','S','P','I'};
  u8 Receivetab[9];

  #ifdef DEBUG
  debug();
  #endif

/* ---------------------------------------------------------------------------------------
   Communication between BSPI0(master) and BSPI1(slave)
      *BSPI0 send nine words of Sendtab to BSPI1 : BSPI0 ----- BSPI ----> BSPI1
      *BSPI1 receive nine words from BSPI0 in Receivetab : BSPI1 <---- BSPI ---- BSPI0
--------------------------------------------------------------------------------------- */

  /* Configure GPI00 on mode Alternate function Push Pull */
  GPIO_Config (GPIO0, 0x00FF, GPIO_AF_PP);

/* -------------------------------------------
Configure BSPI0 as a Master
------------------------------------------- */
  /* Enable the BSPI0 interface */
  BSPI_BSPI0Conf(ENABLE);

  /* Initialize BSPI0 */
  BSPI_Init   ( BSPI0 ) ;

  /* Configure Baud rate Frequency: ---> APB1/6 */
  BSPI_ClockDividerConfig ( BSPI0, 6);

  /* Enable BSPI0 */
  BSPI_Enable ( BSPI0 , ENABLE );

  /* Configure BSPI0 as a Master */
  BSPI_MasterEnable ( BSPI0,ENABLE);

  /* Configure the clock to be active high */
  BSPI_ClkActiveHigh(BSPI0,ENABLE);

  /* Enable capturing the first Data sample on the first edge of SCK */
  BSPI_ClkFEdge(BSPI0,DISABLE);

  /* Set the word length to 16 bit */
  BSPI_8bLEn(BSPI0,ENABLE);

  /*  Configure the depth of transmit to 9 words */
  BSPI_TrFifoDepth(BSPI0,9);
  /* Point on the word to transit */

/* -------------------------------------------
Configue BSPI1 as a Slave
------------------------------------------- */
  /* Initialize BSPI1 */
  BSPI_Init ( BSPI1 ) ;

  /* Configure Baud rate Frequency :-->APB1/8 */
  BSPI_ClockDividerConfig ( BSPI1,8);

  /* Enable BSPI1 */
  BSPI_Enable ( BSPI1 , ENABLE );

  /* Configure BSPI0 as a Slave */
  BSPI_MasterEnable ( BSPI1,DISABLE);

  /* Configure the clock to be active high */
  BSPI_ClkActiveHigh(BSPI1,ENABLE);

  /* Enable capturing the first Data sample on the first edge of SCK */
  BSPI_ClkFEdge(BSPI1,DISABLE);

  /* Set the word length to 16 bit */
  BSPI_8bLEn(BSPI1,ENABLE);

  /* Configure the depth of receive to 9 word */
  BSPI_RcFifoDepth(BSPI1,9);

/* ---------------------------------------------
Send nine words of buffer ---> BSPI0
--------------------------------------------- */

  BSPI_BufferSend(BSPI0,Sendtab,9);

/* ---------------------------------------------
Receive nine words from BSPI0
--------------------------------------------- */

  BSPI_BufferReceive(BSPI1,Receivetab,9);

  /* Infinite loop */
  while (1);
}



















