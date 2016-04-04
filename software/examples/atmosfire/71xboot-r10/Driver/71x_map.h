/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : 71x_map.h
* Author             : MCD Application Team
* Date First Issued  : 16/05/2003
* Description        : Peripherals memory mapping and registers structures
********************************************************************************
* History:
*  16/05/03 : Created
*******************************************************************************/
#ifndef __71x_map_H
#define __71x_map_H

#ifndef EXT
  #define EXT extern
#endif

#include "71x_conf.h"
#include "71x_type.h"


/* IP registers structures */

typedef volatile struct
{
  u16 DATA0;
  u16 EMPTY1[3];
  u16 DATA1;
  u16 EMPTY2[3];
  u16 DATA2;
  u16 EMPTY3[3];
  u16 DATA3;
  u16 EMPTY4[3];
  u16 CSR;
  u16 EMPTY5[7];
  u16 PRES;
} ADC12_TypeDef;

typedef volatile struct
{
  u32 BSR;
  u32 TOR;
  u32 OMR;
  u32 TOER;
  u32 CKDIS;
  u32 SWRES;
} APB_TypeDef;

typedef volatile struct
{
  u16 RXR;
  u16 EMPTY1;
  u16 TXR;
  u16 EMPTY2;
  u16 CSR1;
  u16 EMPTY3;
  u16 CSR2;
  u16 EMPTY4;
  u16 CLK;
} BSPI_TypeDef;

typedef volatile struct
{
  u16 COMR;
  u16 EMPTY1;
  u16 COMM;
  u16 EMPTY2;
  u16 MASK1;
  u16 EMPTY3;
  u16 MASK2;
  u16 EMPTY4;
  u16 ARB1;
  u16 EMPTY5;
  u16 ARB2;
  u16 EMPTY6;
  u16 MSGC;
  u16 EMPTY7;
  u16 DA1;
  u16 EMPTY8;
  u16 DA2;
  u16 EMPTY9;
  u16 DB1;
  u16 EMPTY10;
  u16 DB2;
  u16 EMPTY11[27];
} CAN_MsgObj_TypeDef;

typedef volatile struct
{
  u16 CR;
  u16 EMPTY1;
  u16 SR;
  u16 EMPTY2;
  u16 ERR;
  u16 EMPTY3;
  u16 BTR;
  u16 EMPTY4;
  u16 INTR;
  u16 EMPTY5;
  u16 TESTR;
  u16 EMPTY6;
  u16 BRPR;
  u16 EMPTY7[3];
  CAN_MsgObj_TypeDef sMsgObj[2];
  u16 EMPTY8[16];
  u16 TR1;
  u16 EMPTY9;
  u16 TR2;
  u16 EMPTY10[13];
  u16 NEWD1;
  u16 EMPTY11;
  u16 NEWD2;
  u16 EMPTY12[13];
  u16 INTP1;
  u16 EMPTY13;
  u16 INTP2;
  u16 EMPTY14[13];
  u16 MSGV1;
  u16 EMPTY15;
  u16 MSGV2;
  u16 EMPTY16;
} CAN_TypeDef;

typedef volatile struct
{
  u32 ICR;
  u32 CICR;
  u32 CIPR;
  u32 EMPTY1[3];
  u32 IVR;
  u32 FIR;
  u32 IER;
  u32 EMPTY2[7];
  u32 IPR;
  u32 EMPTY3[7];
  u32 SIR[32];
} EIC_TypeDef;

typedef volatile struct
{
  u16 BCR0;
  u16 EMPTY1;
  u16 BCR1;
  u16 EMPTY2;
  u16 BCR2;
  u16 EMPTY3;
  u16 BCR3;
  u16 EMPTY4;
} EMI_TypeDef;

typedef volatile struct
{
  u32 FCR0;
  u32 FCR1;
  u32 FDR0;
  u32 FDR1;
  u32 FAR;
  u32 FER;
} FLASHR_TypeDef;

typedef volatile struct
{
  u32 FNVWPAR;
  u32 EMPTY;
  u32 FNVAPR0;
  u32 FNVAPR1;
} FLASHPR_TypeDef;

typedef volatile struct
{
  u16 PC0;
  u16 EMPTY1;
  u16 PC1;
  u16 EMPTY2;
  u16 PC2;
  u16 EMPTY3;
  u16 PD;
  u16 EMPTY4;
} GPIO_TypeDef;

typedef volatile struct
{
  u8  CR;
  u8  EMPTY1[3];
  u8  SR1;
  u8  EMPTY2[3];
  u8  SR2;
  u8  EMPTY3[3];
  u8  CCR;
  u8  EMPTY4[3];
  u8  OAR1;
  u8  EMPTY5[3];
  u8  OAR2;
  u8  EMPTY6[3];
  u8  DR;
  u8  EMPTY7[3];
  u8  ECCR;
} I2C_TypeDef;

typedef volatile struct
{
  u16 CPUDIV;
  u16 EMPTY1;
  u16 APBDIV;
  u16 EMPTY2;
  u16 PH_RST;
  u16 EMPTY3;
  u16 PLL2CTL;
  u16 EMPTY4;
  u16 BOOTCONF;
  u16 EMPTY5;
  u16 PWRCTRL;
} PCU_TypeDef;

typedef volatile struct
{
  u32 CLKCTL;
  u32 EMPTY1;
  u32 CLK_FLAG;
  u32 EMPTY2;
  u32 EMPTY3;
  u32 EMPTY4;
  u32 PLLCONF;
  u32 PH_CLOCK_EN;
  u32 SYSMODE;
} RCCU_TypeDef;

typedef volatile struct
{
  u16 CRH;
  u16 EMPTY1;
  u16 CRL;
  u16 EMPTY2;
  u16 LHI;
  u16 EMPTY3;
  u16 LLO;
  u16 EMPTY4;
  u16 DHI;
  u16 EMPTY5;
  u16 DLO;
  u16 EMPTY6;
  u16 CNT_H;
  u16 EMPTY7;
  u16 CNT_L;
  u16 EMPTY8;
  u16 AHI;
  u16 EMPTY9;
  u16 ALO;
} RTC_TypeDef;

typedef volatile struct
{
  u16 ICAR;
  u16 EMPTY1;
  u16 ICBR;
  u16 EMPTY2;
  u16 OCAR;
  u16 EMPTY3;
  u16 OCBR;
  u16 EMPTY4;
  u16 CNTR;
  u16 EMPTY5;
  u16 CR1;
  u16 EMPTY6;
  u16 CR2;
  u16 EMPTY7;
  u16 SR;
} TIM_TypeDef;

typedef volatile struct
{
  u16 BRR;
  u16 EMPTY1;
  u16 TBR;
  u16 EMPTY2;
  u16 RBR;
  u16 EMPTY3;
  u16 CR;
  u16 EMPTY4;
  u16 IER;
  u16 EMPTY5;
  u16 SR;
  u16 EMPTY6;
  u16 GTR;
  u16 EMPTY7;
  u16 TOR;
  u16 EMPTY8;
  u16 TRR;
  u16 EMPTY9;
  u16 RRR;
} UART_TypeDef;

typedef volatile struct
{
  u32 ENDP_0;
  u32 ENDP_1;
  u32 ENDP_2;
  u32 ENDP_3;
  u32 ENDP_4;
  u32 ENDP_5;
  u32 ENDP_6;
  u32 ENDP_7;
  u32 ENDP_8;
  u32 ENDP_9;
  u32 ENDP_10;
  u32 ENDP_11;
  u32 ENDP_12;
  u32 ENDP_13;
  u32 ENDP_14;
  u32 ENDP_15;
  u32 CNTR_;
  u32 ISTR_;
  u32 FNR_;
  u32 DADDR_;
  u32 BTABLE_;
} USB_TypeDef;

typedef volatile struct
{
  u16 CR;
  u16 EMPTY1;
  u16 PR;
  u16 EMPTY2;
  u16 VR;
  u16 EMPTY3;
  u16 CNT;
  u16 EMPTY4;
  u16 SR;
  u16 EMPTY5;
  u16 MR;
  u16 EMPTY6;
  u16 KR;
} WDG_TypeDef;

typedef volatile struct
{
  u8  SRL;
  u8  EMPTY1[7];
  u8  CTRL;
  u8  EMPTY2[3];
  u8  MRH;
  u8  EMPTY3[3];
  u8  MRL;
  u8  EMPTY4[3];
  u8  TRH;
  u8  EMPTY5[3];
  u8  TRL;
  u8  EMPTY6[3];
  u8  PRH;
  u8  EMPTY7[3];
  u8  PRL;
} XTI_TypeDef;


/* IRQ vectors */
typedef volatile struct
{
  u32 T0TIMI_IRQHandler;
  u32 FLASH_IRQHandler;
  u32 RCCU_IRQHandler;
  u32 RTC_IRQHandler;
  u32 WDG_IRQHandler;
  u32 XTI_IRQHandler;
  u32 USBHP_IRQHandler;
  u32 I2C0ITERR_IRQHandler;
  u32 I2C1ITERR_IRQHandler;
  u32 UART0_IRQHandler;
  u32 UART1_IRQHandler;
  u32 UART2_IRQHandler;
  u32 UART3_IRQHandler;
  u32 BSPI0_IRQHandler;
  u32 BSPI1_IRQHandler;
  u32 I2C0_IRQHandler;
  u32 I2C1_IRQHandler;
  u32 CAN_IRQHandler;
  u32 ADC12_IRQHandler;
  u32 T1TIMI_IRQHandler;
  u32 T2TIMI_IRQHandler;
  u32 T3TIMI_IRQHandler;
  u32 EMPTY1[3];
  u32 HDLC_IRQHandler;
  u32 USBLP_IRQHandler;
  u32 EMPTY2[2];
  u32 T0TOI_IRQHandler;
  u32 T0OC1_IRQHandler;
  u32 T0OC2_IRQHandler;
} IRQVectors_TypeDef;

/*===================================================================*/

/* Memory mapping */

#define RAM_BASE        0x20000000

#define FLASHR_BASE     0x40100000
#define FLASHPR_BASE    0x4010DFB0

#define EXTMEM_BASE     0x60000000
#define RCCU_BASE       0xA0000000
#define PCU_BASE        0xA0000040
#define APB1_BASE       0xC0000000
#define APB2_BASE       0xE0000000
#define EIC_BASE        0xFFFFF800

#define I2C0_BASE       (APB1_BASE + 0x1000)
#define I2C1_BASE       (APB1_BASE + 0x2000)
#define UART0_BASE      (APB1_BASE + 0x4000)
#define UART1_BASE      (APB1_BASE + 0x5000)
#define UART2_BASE      (APB1_BASE + 0x6000)
#define UART3_BASE      (APB1_BASE + 0x7000)
#define CAN_BASE        (APB1_BASE + 0x9000)
#define BSPI0_BASE      (APB1_BASE + 0xA000)
#define BSPI1_BASE      (APB1_BASE + 0xB000)
#define USB_BASE        (APB1_BASE + 0x8800)

#define XTI_BASE        (APB2_BASE + 0x101C)
#define GPIO0_BASE      (APB2_BASE + 0x3000)
#define GPIO1_BASE      (APB2_BASE + 0x4000)
#define GPIO2_BASE      (APB2_BASE + 0x5000)
#define ADC12_BASE      (APB2_BASE + 0x7000)
#define TIM0_BASE       (APB2_BASE + 0x9000)
#define TIM1_BASE       (APB2_BASE + 0xA000)
#define TIM2_BASE       (APB2_BASE + 0xB000)
#define TIM3_BASE       (APB2_BASE + 0xC000)
#define RTC_BASE        (APB2_BASE + 0xD000)
#define WDG_BASE        (APB2_BASE + 0xE000)

#define EMI_BASE        (EXTMEM_BASE + 0x0C000000)

/*===================================================================*/

/* IP data access */

#ifndef DEBUG
  #define ADC12 ((ADC12_TypeDef *)ADC12_BASE)

  #define APB1  ((APB_TypeDef *)APB1_BASE)
  #define APB2  ((APB_TypeDef *)APB2_BASE)

  #define BSPI0 ((BSPI_TypeDef *)BSPI0_BASE)
  #define BSPI1 ((BSPI_TypeDef *)BSPI1_BASE)

  #define CAN   ((CAN_TypeDef *)CAN_BASE)

  #define EIC   ((EIC_TypeDef *)EIC_BASE)

  #define EMI   ((EMI_TypeDef *)EMI_BASE)

  #define FLASHR  ((FLASHR_TypeDef *)FLASHR_BASE)
  #define FLASHPR ((FLASHPR_TypeDef *)FLASHPR_BASE)

  #define GPIO0 ((GPIO_TypeDef *)GPIO0_BASE)
  #define GPIO1 ((GPIO_TypeDef *)GPIO1_BASE)
  #define GPIO2 ((GPIO_TypeDef *)GPIO2_BASE)

  #define I2C0  ((I2C_TypeDef *)I2C0_BASE)
  #define I2C1  ((I2C_TypeDef *)I2C1_BASE)

  #define PCU   ((PCU_TypeDef *)PCU_BASE)

  #define RCCU  ((RCCU_TypeDef *)RCCU_BASE)

  #define RTC   ((RTC_TypeDef *)RTC_BASE)

  #define TIM0  ((TIM_TypeDef *)TIM0_BASE)
  #define TIM1  ((TIM_TypeDef *)TIM1_BASE)
  #define TIM2  ((TIM_TypeDef *)TIM2_BASE)
  #define TIM3  ((TIM_TypeDef *)TIM3_BASE)

  #define UART0 ((UART_TypeDef *)UART0_BASE)
  #define UART1 ((UART_TypeDef *)UART1_BASE)
  #define UART2 ((UART_TypeDef *)UART2_BASE)
  #define UART3 ((UART_TypeDef *)UART3_BASE)

  #define USB   ((USB_TypeDef *)USB_BASE)

  #define WDG   ((WDG_TypeDef *)WDG_BASE)

  #define XTI   ((XTI_TypeDef *)XTI_BASE)

  #define IRQVectors ((IRQVectors_TypeDef *)&T0TIMI_Addr)

#else   /* DEBUG */

  #ifdef _ADC12
  EXT ADC12_TypeDef *ADC12;
  #endif

  #ifdef _APB
  #ifdef _APB1
  EXT APB_TypeDef *APB1;
  #endif
  #ifdef _APB2
  EXT APB_TypeDef *APB2;
  #endif
  #endif

  #ifdef _BSPI
  #ifdef _BSPI0
  EXT BSPI_TypeDef *BSPI0;
  #endif
  #ifdef _BSPI1
  EXT BSPI_TypeDef *BSPI1;
  #endif
  #endif

  #ifdef _CAN
  EXT CAN_TypeDef *CAN;
  #endif

  #ifdef _EIC
  EXT EIC_TypeDef *EIC;
  #endif

  #ifdef _EMI
  EXT EMI_TypeDef *EMI;
  #endif

  #ifdef _FLASH
  EXT FLASHR_TypeDef *FLASHR;
  EXT FLASHPR_TypeDef *FLASHPR;
  #endif

  #ifdef _GPIO
  #ifdef _GPIO0
  EXT GPIO_TypeDef *GPIO0;
  #endif
  #ifdef _GPIO1
  EXT GPIO_TypeDef *GPIO1;
  #endif
  #ifdef _GPIO2
  EXT GPIO_TypeDef *GPIO2;
  #endif
  #endif

  #ifdef _I2C
  #ifdef _I2C0
  EXT I2C_TypeDef *I2C0;
  #endif
  #ifdef _I2C1
  EXT I2C_TypeDef *I2C1;
  #endif
  #endif

  #ifdef _PCU
  EXT PCU_TypeDef *PCU;
  #endif

  #ifdef _RCCU
  EXT RCCU_TypeDef *RCCU;
  #endif

  #ifdef _RTC
  EXT RTC_TypeDef *RTC;
  #endif

  #ifdef _TIM
  #ifdef _TIM0
  EXT TIM_TypeDef *TIM0;
  #endif
  #ifdef _TIM1
  EXT TIM_TypeDef *TIM1;
  #endif
  #ifdef _TIM2
  EXT TIM_TypeDef *TIM2;
  #endif
  #ifdef _TIM3
  EXT TIM_TypeDef *TIM3;
  #endif
  #endif

  #ifdef _UART
  #ifdef _UART0
  EXT UART_TypeDef *UART0;
  #endif
  #ifdef _UART1
  EXT UART_TypeDef *UART1;
  #endif
  #ifdef _UART2
  EXT UART_TypeDef *UART2;
  #endif
  #ifdef _UART3
  EXT UART_TypeDef *UART3;
  #endif
  #endif

  #ifdef _USB
  EXT USB_TypeDef *USB;
  #endif

  #ifdef _WDG
  EXT WDG_TypeDef *WDG;
  #endif

  #ifdef _XTI
  EXT XTI_TypeDef *XTI;
  #endif

  #ifdef _IRQVectors
  EXT IRQVectors_TypeDef *IRQVectors;
  #endif

#endif  /* DEBUG */

#endif  /* __71x_map_H */

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
