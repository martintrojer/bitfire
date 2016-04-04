/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : 71x_it.h
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : Interrupt handlers
********************************************************************************
* History:
*  16/05/2003 : Created
*******************************************************************************/
#ifndef _71x_IT_H
#define _71x_IT_H

#include "71x_lib.h"


void Undefined_Handler   (void);
void FIQ_Handler         (void);
void SWI_Handler         (void);
void Prefetch_Handler    (void);
void Abort_Handler       (void);
void T0TIMI_IRQHandler   (void);
void FLASH_IRQHandler    (void);
void RCCU_IRQHandler     (void);
void RTC_IRQHandler      (void);
void WDG_IRQHandler      (void);
void XTI_IRQHandler      (void);
void USBHP_IRQHandler    (void);
void I2C0ITERR_IRQHandler(void);
void I2C1ITERR_IRQHandler(void);
void UART0_IRQHandler    (void);
void UART1_IRQHandler    (void);
void UART2_IRQHandler    (void);
void UART3_IRQHandler    (void);
void BSPI0_IRQHandler    (void);
void BSPI1_IRQHandler    (void);
void I2C0_IRQHandler     (void);
void I2C1_IRQHandler     (void);
void CAN_IRQHandler      (void);
void ADC12_IRQHandler    (void);
void T1TIMI_IRQHandler   (void);
void T2TIMI_IRQHandler   (void);
void T3TIMI_IRQHandler   (void);
void HDLC_IRQHandler     (void);
void USBLP_IRQHandler    (void);
void T0TOI_IRQHandler    (void);
void T0OC1_IRQHandler    (void);
void T0OC2_IRQHandler    (void);

#endif /* _71x_IT_H */

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
