/* Edited by Martin Trojer (mtrojer@arrownordic.com) */
/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : gpio.h
* Author             : MCD Application Team
* Date First Issued  : 08/06/2003
* Description        : This file contains all the functions prototypes for the
*                      GPIO software library.
********************************************************************************
* History:
*  13/01/2006 : V3.1
*  24/05/2005 : V3.0
*  30/11/2004 : V2.0
*  14/07/2004 : V1.3
*  01/01/2004 : V1.2
*******************************************************************************
 THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
 CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
 AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
 OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

#include "71x_map.h"

typedef enum
{
  GPIO_HI_AIN_TRI, /*HIGH IMPEDANCE ANALOG INPUT TRISTATE*/
  GPIO_IN_TRI_TTL, /* INPUT TRISTATE TTL*/
  GPIO_IN_TRI_CMOS,/*INPUT TRSISTATE CMOS*/
  GPIO_IPUPD_WP,   /*INPUT PULL UP/PULL DOWN WEAK PUSH PULL*/
  GPIO_OUT_OD,     /* OUTPUT OPEN DRAIN*/
  GPIO_OUT_PP,     /* OUTPUT PUSH PULL*/
  GPIO_AF_OD,      /*ALTERNATE FUNCTION OPEN DRAIN */
  GPIO_AF_PP       /* ALTERNATE FUNCTION PUSH PULL*/
} GpioPinMode_TypeDef;

#define GPIO_LSB  0x00
#define GPIO_MSB  0x08

/*******************************************************************************
* Function Name  : GPIO_Config
* Description    : Configure the GPIO port pins.
* Input 1        : GPIOx (x can be 0,1 or 2) the desired port.
* Input 2        : Port_Pins : pins placements.
* Input 3        : Pins Mode(GPIO_HI_AIN_TRI,GPIO_IN_TRI_TTL,GPIO_IN_TRI_CMOS,
*                   GPIO_IPUPD_WP,GPIO_OUT_OD,GPIO_OUT_PP,GPIO_AF_OD,GPIO_AF_PP).
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Config (GPIO_TypeDef *GPIOx, u16 Port_Pins, GpioPinMode_TypeDef GPIO_Mode);

/*******************************************************************************
* Function Name  : GPIO_BitRead
* Description    : Read the desired port pin value.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Pin number.
* Output         : None
* Return         : The selected port pin value.
*******************************************************************************/
static inline u8 GPIO_BitRead(GPIO_TypeDef *GPIOx, u8 Port_Pin)
{
  return (GPIOx->PD >> Port_Pin) & 0x0001;
}

/*******************************************************************************
* Function Name  : GPIO_ByteRead
* Description    : Read the desired port Byte value.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Port_Byte(GPIO_MSB or GPIO_LSB).
* Output         : None
* Return         : The GPIO_MSB or GPIO_LSB of the selected PD register.
*******************************************************************************/
static inline u8 GPIO_ByteRead(GPIO_TypeDef *GPIOx, u8 Port_Byte)
{
  return (u8)(GPIOx->PD >> Port_Byte);
}

/*******************************************************************************
* Function Name  : GPIO_WordRead
* Description    : Read the desired port word value.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Output         : None
* Return         : The selected PD register value.
*******************************************************************************/
static inline u16 GPIO_WordRead(GPIO_TypeDef *GPIOx)
{
  return GPIOx->PD;
}

/*******************************************************************************
* Function Name  : GPIO_BitWrite
* Description    : Set or reset the selected port pin.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Pin number.
* Input 3        : bit value.
* Output         : None
* Return         : None.
*******************************************************************************/
void GPIO_BitWrite(GPIO_TypeDef *GPIOx, u8 Port_Pin, u8 Port_Val);

/*******************************************************************************
* Function Name  : GPIO_ByteWrite
* Description    : Write byte value to the selected PD register.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Port_Byte (GPIO_MSB or GPIO_LSB).
* Input 3        : Byte value.
* Output         : None
* Return         : None.
*******************************************************************************/
void GPIO_ByteWrite(GPIO_TypeDef *GPIOx, u8 Port_Byte, u8 Port_Val);

/*******************************************************************************
* Function Name  : GPIO_WordWrite
* Description    : Write word value to the selected PD register.
* Input 1        : Selected GPIO port(GPIO1, GPIO1, GPIO2).
* Input 2        : Value.
* Output         : None
* Return         : None.
*******************************************************************************/
static inline void GPIO_WordWrite(GPIO_TypeDef *GPIOx, u16 Port_Val)
{
  GPIOx->PD = Port_Val;
}

#endif /* __GPIO_H */

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
