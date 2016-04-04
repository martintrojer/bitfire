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

int main(void)
{
  #ifdef DEBUG
  debug();
  #endif

  RCCU_Div2Config(ENABLE);
  RCCU_MCLKConfig(RCCU_DEFAULT);
  RCCU_FCLKConfig(RCCU_RCLK_2);
  RCCU_PCLKConfig(RCCU_RCLK_2);
    
  RCCU_PLL1Config(RCCU_PLL1_Mul_16,RCCU_Div_2);
  RCCU_RCLKSourceConfig(RCCU_PLL1_Output);

  {
    unsigned int kalle;
    {
      volatile unsigned int *addr;
      unsigned int val;
      
      addr = 0x62000000;
      *addr = 0x12345678;
      addr = 0x62080000;
      val = *addr;

      for (addr=0x62000000; addr<0x62100000; addr+=4) {
	*addr = 0x55555555;
	val = *addr;
	if (val != 0x55555555)
	  goto error;
	*addr = 0xaaaaaaaa;
	val = *addr;
	if (val != 0xaaaaaaaa)
	  goto error;
      }
    }

    {
      volatile unsigned short *addr;
      unsigned short val;
      
      addr = 0x62000000;
      *addr = 0x1234;
      
      addr = 0x62080000;
      val = addr;
      
      for (addr=0x62000000; addr<0x62100000; addr+=2) {
	*addr = 0xaaaa;
	val = *addr;
	if (val != 0xaaaa)
	  goto error;
	*addr = 0x5555;
	val = *addr;
	if (val != 0x5555)
	  goto error;
      }
    }

  fine:
    kalle = 0x62000000;

  error:
    kalle = 0x62000000;    
  }

	while(1)
		;  
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
