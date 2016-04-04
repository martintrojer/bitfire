/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file contains a basic example of the use of the
*                      Flash software Library.
********************************************************************************
* History:
*  13/01/2006 : V3.1
*  24/05/2005 : V3.0
*  30/11/2004 : V2.0
*  16/05/2003 : Created
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Includes-------------------------------------------------------------------*/
#include "71x_lib.h"
/* Include of other module interface headers ---------------------------------*/
#include <stdio.h> 

int main(void)
{
 u32 Read_Data;
  #ifdef DEBUG
  debug();
  #endif
  
  /*  Initialize the FLASH */
  FLASH_Init () ;

  /*  Disable temporarily the FLASH protection on Bank 1 sector 0 */
  FLASH_WritePrConfig (FLASH_B0F7,DISABLE) ;

  /*  Erase Bank 1 sector 0 */
  FLASH_SectorErase (FLASH_B0F7) ;

  /*  Write  0x12345678 to Bank 1 sector 0 */
  FLASH_WordWrite (0x00030000,0x12345678) ;

  /* Read back the written data at address 0x400C0000*/     
   Read_Data = FLASH_WordRead(0x00030000);

  /* Display the read data on the debugger output window. You can use also 
     the debugger memory window to verify the good content of the flash at
     0x400C0000 address and see the value "0x12345678"*/
   printf(" The data written at address 0x400C0000 is %x\n",Read_Data);
   printf("\n");
  
  /* Warning!! : The write protection can be enabled ONLY on a permanent way */

  /*  Enable the FLASH protection on Bank 1 sector 0 */
  /*  FLASH_WritePrConfig (FLASH_B1F0,ENABLE) ; */
 
  while(1);

}
/*******************(C)COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
