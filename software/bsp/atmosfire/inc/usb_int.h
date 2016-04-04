/******************** (C) COPYRIGHT 2006 STMicroelectronics ********************
* File Name          : usb_int.h
* Author             : MCD Application Team
* Date First Issued  : 10/27/2003 : V1.0
* Description        : Endpoint CTR (Low and High) interrupt's service routines
*                      prototypes
********************************************************************************
* History:
* 09/18/2006 : V3.0
* 09/01/2006 : V2.0
* 10/27/2003 : V1.0
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_INT_H
#define __USB_INT_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CTR_LP(void);
void CTR_HP(void);
/* External variables --------------------------------------------------------*/
#endif /* __USB_INT_H */
/******************* (C) COPYRIGHT 2006 STMicroelectronics *****END OF FILE****/