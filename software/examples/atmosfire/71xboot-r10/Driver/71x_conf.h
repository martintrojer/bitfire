/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : 71x_conf.h
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : Library configuration
********************************************************************************
* History:
*  16/05/03 : Created
*******************************************************************************/
#ifndef __71x_CONF_H
#define __71x_CONF_H


/* Comment the line below to put the library in release mode */
//#define DEBUG

#define inline static inline

//#define USE_SERIAL_PORT
//#define USE_UART0
//#define USE_UART1
//#define USE_UART2
//#define USE_UART3

/* Comment the lines below corresponding to unwanted peripherals */
//#define _ADC12

//#define _APB
//#define _APB1
//#define _APB2

//#define _BSPI
//#define _BSPI0
//#define _BSPI1

//#define _CAN

//#define _EIC

//#define _EMI

//#define _FLASH

#define _GPIO
#define _GPIO0
#define _GPIO1
#define _GPIO2

//#define _I2C
//#define _I2C0
//#define _I2C1

//#define _PCU

#define _RCCU

//#define _RTC

//#define _TIM
//#define _TIM0
//#define _TIM1
//#define _TIM2
//#define _TIM3

#define _UART
//#define _UART0
//#define _UART1
//#define _UART2
#define _UART3

#define UARTX UART3		//Default UART used for console


//#define _USB

#define _WDG

//#define _XTI


//#define _IRQVectors


#endif /* __71x_CONF_H */

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
