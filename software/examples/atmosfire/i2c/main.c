/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : This file is used for I2C library example.
********************************************************************************
* History:
*  13/01/2006 :  V3.1
*  06/06/2005 :  V2
*  24/11/2004 :  V1.2
*  22/09/2003 :  Created
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
/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_SCL 0x2000
#define I2C_SDA 0x4000

#define M24C08_Block0ADDRESS 0xA8
#define M24C08_Block1ADDRESS 0xAA
#define M24C08_Block2ADDRESS 0xAC
#define M24C08_Block3ADDRESS 0xAE

#define STRING "STR7 I2C EEPROM"

#define Buffer_Size (sizeof(STRING)-1)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char *Tx_Data = STRING;
char  Rx_Data[Buffer_Size] = "";
u8 i;
u32 I2CStatus;
/* Private function prototypes -----------------------------------------------*/
void EEPROM_Receive(I2C_TypeDef *I2Cx, char *PtrToBuffer,u8 NbOfBytes, 
                    u8 InternalAddress);
void EEPROM_WaitForLastTask(void);
void EEPROM_Send(I2C_TypeDef *I2Cx,char *PtrToBuffer, u8 NbOfBytes, 
                 u8 InternalAddress);
void I2C0_Config(void);
/* Interface functions -------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int main(void)
{
  #ifdef DEBUG
    debug();
  #endif

/* Print TX buffer */
  printf("Tx_Buffer : %s\n",Tx_Data);

  /* Print RX buffer on the debugger output window */
  printf("Rx_Buffer : ");
  for(i=0;i<Buffer_Size;i++)
    printf("%c",Rx_Data[i]);
  printf("\n");

  /* Configure I2C0 interface */
  I2C0_Config();
  /* Send Buffer to the M24C08 */
  EEPROM_Send(I2C0, Tx_Data,Buffer_Size, 0x00);
  /* Wait until the EEPROM completes the last operation */
  EEPROM_WaitForLastTask();

  while(1)
  {
    /* Read Buffer from the M24C08 */ 
    EEPROM_Receive(I2C0, Rx_Data,Buffer_Size, 0x00);
    /* Print RX buffer on the debugger output window */
    printf("Rx_Buffer : %s\n",Rx_Data);
  }  
}

/*******************************************************************************
* Function Name  : EEPROM_WaitForLastTask
* Description    : Wait for M24C08 EEPROM to terminate the last task.
* Input          : None.
* Return         : None.
*******************************************************************************/
void EEPROM_WaitForLastTask(void)
{
  do
  {
    I2C_STARTGenerate( I2C0, ENABLE );
    while( I2C_FlagStatus( I2C0, DIRECT, I2C_SB) == RESET );
    I2C_AddressSend( I2C0, M24C08_Block3ADDRESS, I2C_Mode7, I2C_TX );
    while(!((I2CStatus = I2C_GetStatus( I2C0 ))& I2C_EVF));
    while( I2C_FlagStatus( I2C0, DIRECT, I2C_ENDAD ) ==RESET );
    I2C_FlagClear( I2C0, I2C_ENDAD );
  }while( I2C_FlagStatus( I2C0, INDIRECT, I2C_AF, I2CStatus ) == SET );
}

/*******************************************************************************
* Function Name  : EEPROM_Receive
* Description    : Read a buffer from the M24C08 EEPROM.
* Input          : I2Cx (I2C0 or I2C1).
*                  Buffer where read data are put.
*                  Buffer size.
*                  EEPROM internal address from which data will be read.
* Return         : None.
*******************************************************************************/

void EEPROM_Receive(I2C_TypeDef *I2Cx, char *PtrToBuffer,u8 NbOfBytes, 
                    u8 InternalAddress)
{

  /* Generate the START condition */
  I2C_STARTGenerate (I2Cx, ENABLE);
  /* Wait until SB bit is set */
  while (I2C_FlagStatus (I2Cx, DIRECT, I2C_SB )== RESET);
  /* Send the EEPROM address with LSB bit reset */
  I2C_AddressSend (I2Cx,M24C08_Block3ADDRESS,I2C_Mode7, I2C_TX);
  /* Wait until ENDAD bit is set */  
  while (I2C_FlagStatus (I2Cx, DIRECT, I2C_ENDAD )== RESET);
  /* Clear ENDAD bit */
  I2C_FlagClear (I2Cx, I2C_ENDAD);
  /* Send the EEPROM's internal address to read from */
  I2C_ByteSend (I2Cx, InternalAddress);
  /* Wait until BTF bit is set */
  while (I2C_FlagStatus (I2Cx, DIRECT, I2C_BTF )==RESET);
  /* Generate the RE-START condition */
  I2C_STARTGenerate (I2Cx, ENABLE);
  /* Wait until SB bit is set */
  while (I2C_FlagStatus (I2Cx, DIRECT, I2C_SB )==RESET);  
  /* Send the EEPROM address with LSB bit set */
  I2C_AddressSend (I2C0, M24C08_Block3ADDRESS, I2C_Mode7, I2C_RX);
  /* Wait until ENDAD bit is set */
  while (I2C_FlagStatus (I2Cx, DIRECT, I2C_ENDAD)==RESET);
  /* Clear ENDAD bit */
  I2C_FlagClear (I2Cx, I2C_ENDAD);
  
/* Read 'NbOfBytes' bytes from the EEPROM starting from address 'InternalAddress' 
of Block3 and place them in PtrToBuffer[] array */

while(NbOfBytes)
{
   /* Wait until the byte is received */
   while (I2C_FlagStatus (I2Cx, DIRECT, I2C_BTF )==RESET);

  if(NbOfBytes==2)
  { /* Disable the ACK generation */
    I2C_AcknowledgeConfig (I2Cx, DISABLE);
  }

  if (NbOfBytes==1)
    {
  /* Generate STOP condition to close the communication after the
     next byte reception */ 
      I2C_STOPGenerate (I2Cx, ENABLE);
    }

  *PtrToBuffer=I2C_ByteReceive (I2Cx);
   PtrToBuffer++;
   NbOfBytes--;
}

  /* Enable the ACK generation */
  I2C_AcknowledgeConfig (I2Cx, ENABLE);
}


/*******************************************************************************
* Function Name  : EEPROM_Send
* Description    : Send data to the M24C08 EEPROM.
* Input          : I2Cx (I2C0 or I2C1).
*                  Buffer containing data to be written in the EEPROM.
*                  Buffer size.
*                  EEPROM internal address in which data will be written.
* Return         : None.
*******************************************************************************/
void EEPROM_Send(I2C_TypeDef *I2Cx, char *PtrToBuffer, u8 NbOfBytes, 
                u8 InternalAddress)
{

u8 SentBytes =0;

  /* Generate the START condition */
  I2C_STARTGenerate (I2C0, ENABLE);
  /* Wait until SB bit is set */    
  while (I2C_FlagStatus (I2C0, DIRECT, I2C_SB)==RESET);
  /* Send the EEPROM's address with LSB bit reset */
  I2C_AddressSend (I2C0, M24C08_Block3ADDRESS, I2C_Mode7, I2C_TX);
  /* Wait until ENDAD bit is set */
  while (I2C_FlagStatus (I2C0, DIRECT, I2C_ENDAD)==RESET);
  /* Clear ENDAD bit */
  I2C_FlagClear (I2C0, I2C_ENDAD);  
  /* Send the EEPROM's internal address to write to */
  I2C_ByteSend (I2C0,InternalAddress);
  /* Wait until BTF bit is set */ 
  while (I2C_FlagStatus (I2C0, DIRECT, I2C_BTF )==RESET);  
  /* Write 'PtrToBuffer' buffer contents in the EEPROM starting from address 'InternalAddress' of
  Block3 */
     
   while (SentBytes<NbOfBytes)
  {
    I2Cx->DR= *(PtrToBuffer+SentBytes);
    /* Wait till I2C_BTF bit is set */
    while (!(I2Cx->SR1 & 0x08));
    SentBytes++;
   }
   /* Generate the stop condition */
  I2C_STOPGenerate (I2C0, ENABLE);
  
 }

/*******************************************************************************
* Function Name  : I2C0_Config
* Description    : Configure the I2C0 interface
* Input          : None
*******************************************************************************/

void I2C0_Config(void)
{
  /* Configure the SDA and the SCL lines to alternate functions Open Drain */
  GPIO_Config (GPIO1, I2C_SCL|I2C_SDA, GPIO_AF_OD);
  /* Initialize the I2C0 peripheral */
  I2C_Init (I2C0);
  /* Configure frequency bits */
  I2C_FCLKConfig (I2C0);
  /* Enable I2C0 peripheral */
  I2C_OnOffConfig (I2C0, ENABLE);
  /* Configure I2C0 clock speed */
  I2C_SpeedConfig (I2C0, 100000);
  /* Enable Acknowledge */
  I2C_AcknowledgeConfig (I2C0, ENABLE);
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
