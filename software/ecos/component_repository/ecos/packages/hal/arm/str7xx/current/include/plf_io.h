#ifndef CYGONCE_HAL_PLF_IO_H
#define CYGONCE_HAL_PLF_IO_H
//=============================================================================
//
//      plf_io.h
//
//      Platform specific registers
//
//=============================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
//
// eCos is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 or (at your option) any later version.
//
// eCos is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with eCos; if not, write to the Free Software Foundation, Inc.,
// 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or inline functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. However the source code for this file must still be made available
// in accordance with section (3) of the GNU General Public License.
//
// This exception does not invalidate any other reasons why a work based on
// this file might be covered by the GNU General Public License.
//
// Alternative licenses for eCos may be arranged by contacting Red Hat, Inc.
// at http://sources.redhat.com/ecos/ecos-license/
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//=============================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):   jskov
// Contributors:jskov, gthomas
// Date:        2001-07-12
// Purpose:     AT91/EB40 platform specific registers
// Description: 
// Usage:       #include <cyg/hal/plf_io.h>
//
//####DESCRIPTIONEND####
//
//=============================================================================

// STR7xx Minimal RTOS lib, drivers ripped from ST driver lib
// Martin Trojer (mtrojer@arrownordic.com)

#ifndef __MINRTOS_H
#define __MINRTOS_H

void minrtos_init(void);
void minrtos_setup_clocks(void);
void minrtos_setup_tick_timer(int period);
void minrtos_setup_uart(int id, int baud); // UART2 or 3 on atmosfire

#define UART2_Rx_Pin (0x0001<<13)  /*  TQFP 64: pin N° 5  , TQFP 144 pin N° 9 */
#define UART2_Tx_Pin (0x0001<<14)  /*  TQFP 64: pin N° 6  , TQFP 144 pin N° 10 */

#define UART3_Rx_Pin (0x0001<<1)   /*  TQFP 64: pin N° 52 , TQFP 144 pin N° 123 */
#define UART3_Tx_Pin (0x0001<<0)   /*  TQFP 64: pin N° 53 , TQFP 144 pin N° 124 */

#define RCCU_Main_Osc 16000000

//=========================================================================================================
//=========================================================================================================
// 71x_type.h
//=========================================================================================================
//=========================================================================================================
typedef unsigned long   u32;
typedef unsigned short  u16;
typedef unsigned char   u8;
typedef signed long   s32;
typedef signed short  s16;
typedef signed char   s8;
typedef volatile unsigned long   vu32;
typedef volatile unsigned short  vu16;
typedef volatile unsigned char   vu8;
typedef volatile signed long   vs32;
typedef volatile signed short  vs16;
typedef volatile signed char   vs8;
//typedef enum { FALSE = 0, TRUE  = !FALSE } bool;
typedef enum { RESET = 0, SET   = !RESET } FlagStatus;
typedef enum { DISABLE = 0, ENABLE  = !DISABLE} FunctionalState;
typedef enum { INDIRECT = 0, DIRECT  = !INDIRECT} RegisterAccess;

//=========================================================================================================
//=========================================================================================================
// 71x_map.h
//=========================================================================================================
//=========================================================================================================
typedef volatile struct
{
  vu16 RXR;
  vu16 EMPTY1;
  vu16 TXR;
  vu16 EMPTY2;
  vu16 CSR1;
  vu16 EMPTY3;
  vu16 CSR2;
  vu16 EMPTY4;
  vu16 CLK;
} BSPI_TypeDef;
typedef volatile struct
{
  vu32 ICR;
  vu32 CICR;
  vu32 CIPR;
  vu32 EMPTY1[3];
  vu32 IVR;
  vu32 FIR;
  vu32 IER;
  vu32 EMPTY2[7];
  vu32 IPR;
  vu32 EMPTY3[7];
  vu32 SIR[32];
} EIC_TypeDef;
typedef volatile struct
{
  vu32 CR0;
  vu32 CR1;
  vu32 DR0;
  vu32 DR1;
  vu32 AR;
  vu32 ER;
} FLASHR_TypeDef;

typedef volatile struct
{
  vu32 NVWPAR;
  vu32 EMPTY;
  vu32 NVAPR0;
  vu32 NVAPR1;
} FLASHPR_TypeDef;
typedef volatile struct
{
  vu16 PC0;
  vu16 EMPTY1;
  vu16 PC1;
  vu16 EMPTY2;
  vu16 PC2;
  vu16 EMPTY3;
  vu16 PD;
  vu16 EMPTY4;
} GPIO_TypeDef;
typedef volatile struct
{
  vu32 CCR;
  vu32 EMPTY1;
  vu32 CFR;
  vu32 EMPTY2[3];
  vu32 PLL1CR;
  vu32 PER;
  vu32 SMR;
} RCCU_TypeDef;
typedef volatile struct
{
  vu16 MDIVR;
  vu16 EMPTY1;
  vu16 PDIVR;
  vu16 EMPTY2;
  vu16 RSTR;
  vu16 EMPTY3;
  vu16 PLL2CR;
  vu16 EMPTY4;
  vu16 BOOTCR;
  vu16 EMPTY5;
  vu16 PWRCR;
} PCU_TypeDef;
typedef volatile struct
{
  vu16 ICAR;
  vu16 EMPTY1;
  vu16 ICBR;
  vu16 EMPTY2;
  vu16 OCAR;
  vu16 EMPTY3;
  vu16 OCBR;
  vu16 EMPTY4;
  vu16 CNTR;
  vu16 EMPTY5;
  vu16 CR1;
  vu16 EMPTY6;
  vu16 CR2;
  vu16 EMPTY7;
  vu16 SR;
} TIM_TypeDef;
typedef volatile struct
{
  vu16 BR;
  vu16 EMPTY1;
  vu16 TxBUFR;
  vu16 EMPTY2;
  vu16 RxBUFR;
  vu16 EMPTY3;
  vu16 CR;
  vu16 EMPTY4;
  vu16 IER;
  vu16 EMPTY5;
  vu16 SR;
  vu16 EMPTY6;
  vu16 GTR;
  vu16 EMPTY7;
  vu16 TOR;
  vu16 EMPTY8;
  vu16 TxRSTR;
  vu16 EMPTY9;
  vu16 RxRSTR;
} UART_TypeDef;

#define RAM_BASE        0x20000000
#define FLASHR_BASE     0x40100000
#define FLASHPR_BASE    0x4010DFB0
#define EXTMEM_BASE     0x60000000
#define RCCU_BASE       0xA0000000
#define PCU_BASE        0xA0000040
#define APB1_BASE       0xC0000000
#define APB2_BASE       0xE0000000
#define EIC_BASE        0xFFFFF800

#define UART0_BASE      (APB1_BASE + 0x4000)
#define UART1_BASE      (APB1_BASE + 0x5000)
#define UART2_BASE      (APB1_BASE + 0x6000)
#define UART3_BASE      (APB1_BASE + 0x7000)
#define BSPI0_BASE      (APB1_BASE + 0xA000)
#define BSPI1_BASE      (APB1_BASE + 0xB000)
#define GPIO0_BASE      (APB2_BASE + 0x3000)
#define GPIO1_BASE      (APB2_BASE + 0x4000)
#define GPIO2_BASE      (APB2_BASE + 0x5000)
#define TIM0_BASE       (APB2_BASE + 0x9000)
#define TIM1_BASE       (APB2_BASE + 0xA000)
#define TIM2_BASE       (APB2_BASE + 0xB000)
#define TIM3_BASE       (APB2_BASE + 0xC000)

#define BSPI0 ((BSPI_TypeDef *)BSPI0_BASE)
#define BSPI1 ((BSPI_TypeDef *)BSPI1_BASE)
#define EIC   ((EIC_TypeDef *)EIC_BASE)
#define FLASHR  ((FLASHR_TypeDef *)FLASHR_BASE)
#define FLASHPR ((FLASHPR_TypeDef *)FLASHPR_BASE)
#define GPIO0 ((GPIO_TypeDef *)GPIO0_BASE)
#define GPIO1 ((GPIO_TypeDef *)GPIO1_BASE)
#define GPIO2 ((GPIO_TypeDef *)GPIO2_BASE)
#define PCU   ((PCU_TypeDef *)PCU_BASE)
#define RCCU  ((RCCU_TypeDef *)RCCU_BASE)
#define TIM0  ((TIM_TypeDef *)TIM0_BASE)
#define TIM1  ((TIM_TypeDef *)TIM1_BASE)
#define TIM2  ((TIM_TypeDef *)TIM2_BASE)
#define TIM3  ((TIM_TypeDef *)TIM3_BASE)
#define UART0 ((UART_TypeDef *)UART0_BASE)
#define UART1 ((UART_TypeDef *)UART1_BASE)
#define UART2 ((UART_TypeDef *)UART2_BASE)
#define UART3 ((UART_TypeDef *)UART3_BASE)

//=========================================================================================================
//=========================================================================================================
// eic.h
//=========================================================================================================
//=========================================================================================================
typedef enum
{
  T0TIMI_IRQChannel    = 0,
  FLASH_IRQChannel     = 1,
  RCCU_IRQChannel      = 2,
  RTC_IRQChannel       = 3,
  WDG_IRQChannel       = 4,
  XTI_IRQChannel       = 5,
  USBHP_IRQChannel     = 6,
  I2C0ITERR_IRQChannel = 7,
  I2C1ITERR_IRQChannel = 8,
  UART0_IRQChannel     = 9,
  UART1_IRQChannel     = 10,
  UART2_IRQChannel     = 11,
  UART3_IRQChannel     = 12,
  SPI0_IRQChannel      = 13,
  SPI1_IRQChannel      = 14,
  I2C0_IRQChannel      = 15,
  I2C1_IRQChannel      = 16,
  CAN_IRQChannel       = 17,
  ADC_IRQChannel       = 18,
  T1TIMI_IRQChannel    = 19,
  T2TIMI_IRQChannel    = 20,
  T3TIMI_IRQChannel    = 21,
  HDLC_IRQChannel      = 25,
  USBLP_IRQChannel     = 26,
  T0TOI_IRQChannel     = 29,
  T0OC1_IRQChannel     = 30,
  T0OC2_IRQChannel     = 31
} IRQChannel_TypeDef;

typedef enum
{
  T0TIMI_FIQChannel     = 0x00000001,
  WDG_FIQChannel        = 0x00000002,
  WDGT0TIMI_FIQChannels = 0x00000003
} FIQChannel_TypeDef;

/*******************************************************************************
* Function Name  : EIC_Init
* Description    : Initialise the EIC using the load PC instruction
*                 (PC = PC +offset)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EIC_Init(void);

/*******************************************************************************
* Function Name  : EIC_IRQConfig
* Description    : Enable or Disable IRQ interrupts
* Input 1        : New status : can be ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
static inline void EIC_IRQConfig(FunctionalState NewState)
{
  if (NewState==ENABLE) EIC->ICR |= 0x0001; else EIC->ICR &= ~0x0001;
}

/*******************************************************************************
* Function Name  : EIC_FIQConfig
* Description    : Enable or Disable FIQ interrupts
* Input 1        : New status : can be ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
static inline void EIC_FIQConfig(FunctionalState NewState)
{
  if (NewState==ENABLE) EIC->ICR |= 0x0002; else EIC->ICR &= ~0x0002;
}

/*******************************************************************************
* Function Name  : EIC_IRQChannelConfig
* Description    : Configure the IRQ Channel
* Input 1        : IRQ channel name
* Input 2        : Channel New status : can be ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
static inline void EIC_IRQChannelConfig(IRQChannel_TypeDef IRQChannel, FunctionalState NewState)
{
  if (NewState==ENABLE) EIC->IER |= 0x0001 << IRQChannel;
  else EIC->IER &= ~(0x0001 << IRQChannel);
}

/*******************************************************************************
* Function Name  : EIC_FIQChannelConfig
* Description    : Configure the FIQ Channel
* Input 1        : FIQ channel name
* Input 2        : Channel New status : can be ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
static inline void EIC_FIQChannelConfig(FIQChannel_TypeDef FIQChannel,
                                        FunctionalState NewState)
{
  if (NewState==ENABLE) EIC->FIR |= FIQChannel;
  else EIC->FIR &= ~FIQChannel;
}

/*******************************************************************************
* Function Name  : EIC_IRQChannelPriorityConfig
* Description    : Configure the selected IRQ channel priority
* Input 1        : IRQ channel name
* Input 2        : IRQ channel priority
* Output         : None
* Return         : None
*******************************************************************************/
static inline void EIC_IRQChannelPriorityConfig(IRQChannel_TypeDef IRQChannel, u8 Priority)
{
  EIC->SIR[IRQChannel] = (EIC->SIR[IRQChannel]&0xFFFF0000) | (u16)Priority & 0x000F;
}

/*******************************************************************************
* Function Name  : EIC_CurrentPriorityLevelConfig
* Description    : Change the current priority level of the srved IRQ routine
* Input 1        : New priority
* Output         : None
* Return         : None
*******************************************************************************/
void EIC_CurrentPriorityLevelConfig(u8 NewPriorityLevel);

/*******************************************************************************
* Function Name  : EIC_CurrentPriorityLevelValue
* Description    : Return the current priority level of the current served IRQ
*                  routine
* Input          : None
* Output         : None
* Return         : The current priority level
*******************************************************************************/
static inline u8 EIC_CurrentPriorityLevelValue(void)
{
  return EIC->CIPR & 0xF;
}

/*******************************************************************************
* Function Name  : EIC_CurrentIRQChannelValue
* Description    : Return the current served IRQ channel number
* Input 0        : None
* Output         : None
* Return         : The current served IRQ channel number
*******************************************************************************/
static inline IRQChannel_TypeDef EIC_CurrentIRQChannelValue(void)
{
  return (IRQChannel_TypeDef)(EIC->CICR & 0x1F);
}

/*******************************************************************************
* Function Name  : EIC_CurrentFIQChannelValue
* Description    : Return the current served FIQ channel number
* Input          : None
* Output         : None
* Return         : The current served FIQ channel number
*******************************************************************************/
static inline FIQChannel_TypeDef EIC_CurrentFIQChannelValue(void)
{
  return (FIQChannel_TypeDef)((EIC->FIR >> 2) & 0x0003);
}

/*******************************************************************************
* Function Name  : EIC_FIPendingBitClear
* Description    : Clear the FIQ pending bit
* Input 1        : FIQ channel
* Output         : None
* Return         : None
*******************************************************************************/
static inline void EIC_FIQPendingBitClear(FIQChannel_TypeDef FIQChannel)
{
  EIC->FIR = (EIC->FIR & 0x0003) | (FIQChannel << 2);
}

//=========================================================================================================
//=========================================================================================================
// gpio.h
//=========================================================================================================
//=========================================================================================================
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

//=========================================================================================================
//=========================================================================================================
// pcu.h
//=========================================================================================================
//=========================================================================================================
typedef enum
{
  PCU_WREN = 0x8000,
  PCU_VROK = 0x1000
} PCU_Flags;

typedef enum
{
  PCU_STABLE,
  PCU_UNSTABLE
} PCU_VR_Status;

typedef enum
{
  PCU_MVR = 0x0008,
  PCU_LPR = 0x0010
} PCU_VR;

typedef enum
{
  WFI_CLOCK2_16,
  WFI_EXTERNAL
} WFI_CLOCKS;

typedef enum
{
  PCU_SLOW,
  PCU_STOP,
  PCU_STANDBY
} LPM_MODES;

/*  VR_OK  : Voltage Regulator OK */
#define PCU_VROK_Mask       0x1000
/*  Main Voltage Regulator */
#define PCU_MVR_Mask        0x0008
/*  Low Power Voltage Regulator */
#define PCU_LPR_Mask        0x0010
/*  PCU register Write Enable Bit */
#define PCU_WREN_Mask       0x8000
/*  PCU register Backup logic Busy ( programming ongoing) Bit */
#define PCU_BUSY_Mask       0x4000
/*  Low Voltage Detector */
#define PCU_LVD_Mask        0x0100
/*  Power Down Flag */
#define PCU_PWRDWN_Mask     0x0040
/*  WFI Mode Clock Selection Bit */
#define PCU_WFI_CKSEL_Mask  0x00000002
/*  Halt Mode Enable Bit */
#define PCU_EN_HALT_Mask    0x00000800
/*  Halt Mode Flag */
#define PCU_HALT_Mask       0x0002
/*  Stop Mode Enable Bit */
#define PCU_STOP_EN_Mask    0x00000400
/*  Low Power Regulator in Wait For interrupt Mode */
#define PCU_LPRWFI_Mask     0x0020
/*  Low Power Mode in Wait For interrupt Mode */
#define PCU_LPOWFI_Mask     0x00000001
/*  Software Reset Enable */
#define PCU_SRESEN_Mask     0x00000001

/*******************************************************************************
* Function Name  : PCU_MVRStatus
* Description    : This routine is used to check the Main Voltage Regulator
*                : NewState.
* Input          : None
* Return         : STABLE, UNSTABLE
*******************************************************************************/
static inline PCU_VR_Status PCU_MVRStatus ( void )
{
  return (PCU->PWRCR & PCU_VROK_Mask) == 0x00 ? PCU_UNSTABLE : PCU_STABLE;
}

/*******************************************************************************
* Function Name  : PCU_FlagStatus
* Description    : This routine is used to return the PCU register flag
* Input 1        : The flag to get
* Return         : RESET, SET
*******************************************************************************/
static inline FlagStatus PCU_FlagStatus ( PCU_Flags Xflag )
{
  return ( PCU->PWRCR & Xflag ) == 0x00 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : PCU_VRConfig
* Description    : This routine is used to configure PCU voltage regultors
* Input 1        : MVR : Main voltage Regulator
                   LPR : Low Power Regulator
* Input 2        : ENABLE : Enable the Voltage Regulator
                   DISABLE: Disable ( ByPass ) the VR
* Return         : None
*******************************************************************************/
void PCU_VRConfig ( PCU_VR xVR, FunctionalState NewState );

/*******************************************************************************
* Function Name  : PCU_VRStatus
* Description    : This routine is used to get the PCU voltage regultors status
* Input          : MVR : Main voltage Regulator
                   LPR : Low Power Regulator
* Return         : ENABLE : Enable the Voltage Regulator
                   DISABLE: Disable ( ByPass ) the VR
*******************************************************************************/
static inline FunctionalState PCU_VRStatus ( PCU_VR xVR )
{
  return ( PCU->PWRCR & xVR ) == 0  ? ENABLE : DISABLE;
}

/*******************************************************************************
* Function Name  : PCU_LVDDisable
* Description    : This routine is used to disable the Low Voltage Detector.
* Input          : None
* Return         : None
*******************************************************************************/
static inline void PCU_LVDDisable ( void )
{
  /*  Wait until the previous write operation will be completed */
  while (( PCU->PWRCR & PCU_BUSY_Mask ) == 1);

  /*  Unlock Power Control Register */
  PCU->PWRCR |= PCU_WREN_Mask;

  /*  Set the LVD DIS Flag */
  PCU->PWRCR |= PCU_LVD_Mask;
}

/*******************************************************************************
* Function Name  : PCU_LVDStatus
* Description    : This routine is used to get the LVD NewState.
* Input          : None
* Return         : ENABLE, DISABLE
*******************************************************************************/
static inline FunctionalState PCU_LVDStatus ( void )
{
  return ( PCU->PWRCR & PCU_LVD_Mask ) == 0 ? ENABLE : DISABLE;
}

/*******************************************************************************
* Function Name  : PCU_WFIEnter
* Description    : This routine is used to force the Device to enter in WFI mode
* Input 1        : CLOCK2_16 : Clock2_16 as system clock for WFI mode
*                  EXTERNAL  : external clock as system clock for WFI mode
* Input 2        : ENABLE : Enable Low Power Regulator during Wait For Interrupt Mode
*                  DISABLE: Disable Low Power Regulator during Wait For Interrupt Mode
* Input 3        : ENABLE : Enable Low Power Mode during Wait For Interrupt Mode
*                  DISABLE: Disable Low Power Mode during Wait For Interrupt Mode
* Return         : None
*******************************************************************************/
void PCU_WFIEnter ( WFI_CLOCKS Xclock, FunctionalState Xlpr, FunctionalState Xlpm );

/*******************************************************************************
* Function Name  : PCU_LPMEnter
* Description    : This routine is used to force the Device to enter low
*                  power mode
* Input          : SLOW : Slow Mode
                   STOP : Stop Mode
                   HALT : Halt Mode
* Return         : None
*******************************************************************************/
void PCU_LPMEnter ( LPM_MODES Xmode);

//=========================================================================================================
//=========================================================================================================
// rccu.h
//=========================================================================================================
//=========================================================================================================
typedef enum {
  RCCU_DEFAULT=0x00,
  RCCU_RCLK_2 =0x01,
  RCCU_RCLK_4 =0x02,
  RCCU_RCLK_8 =0x03
} RCCU_Clock_Div;

typedef enum {
  RCCU_PLL1_Output,
  RCCU_CLOCK2_16,
  RCCU_CLOCK2,
  RCCU_RTC_CLOCK
} RCCU_RCLK_Clocks;


typedef enum {
  RCCU_PLL1_Mul_12=0x01,
  RCCU_PLL1_Mul_16=0x03,
  RCCU_PLL1_Mul_20=0x00,
  RCCU_PLL1_Mul_24=0x02
} RCCU_PLL1_Mul;

typedef enum {
  RCCU_PLL2_Mul_12=0x01,
  RCCU_PLL2_Mul_16=0x03,
  RCCU_PLL2_Mul_20=0x00,
  RCCU_PLL2_Mul_28=0x02
} RCCU_PLL2_Mul;

typedef enum {
  RCCU_Div_1=0x00,
  RCCU_Div_2=0x01,
  RCCU_Div_3=0x02,
  RCCU_Div_4=0x03,
  RCCU_Div_5=0x04,
  RCCU_Div_6=0x05,
  RCCU_Div_7=0x06
} RCCU_PLL_Div;

typedef enum {
  RCCU_PLL2_Output = 0x01,
  RCCU_USBCK       = 0x00
} RCCU_USB_Clocks;

typedef enum {
  RCCU_CLK2,
  RCCU_RCLK,
  RCCU_MCLK,
  RCCU_PCLK,
  RCCU_FCLK
} RCCU_Clocks;

typedef enum {
  RCCU_PLL1_LOCK_IT = 0x0080,
  RCCU_CKAF_IT      = 0x0100,
  RCCU_CK2_16_IT    = 0x0200,
  RCCU_STOP_IT      = 0x0400
} RCCU_Interrupts;

typedef enum {
  RCCU_PLL1_LOCK   = 0x0002,
  RCCU_CKAF_ST     = 0x0004,
  RCCU_PLL1_LOCK_I = 0x0800,
  RCCU_CKAF_I      = 0x1000,
  RCCU_CK2_16_I    = 0x2000,
  RCCU_STOP_I      = 0x4000
} RCCU_Flags;

typedef enum {
  RCCU_ExternalReset = 0x00000000,
  RCCU_SoftwareReset = 0x00000020,
  RCCU_WDGReset      = 0x00000040,
  RCCU_RTCAlarmReset = 0x00000080,
  RCCU_LVDReset      = 0x00000200,
  RCCU_WKPReset      = 0x00000400
}RCCU_ResetSources;


#define RCCU_Div2_Mask  0x00008000
#define RCCU_Div2_Index 0x0F
#define RCCU_FACT_Mask  0x0003

#define RCCU_FACT1_Mask  0x0003

#define RCCU_FACT2_Mask  0x0300
#define RCCU_FACT2_Index 0x08

#define RCCU_MX_Mask   0x00000030
#define RCCU_MX_Index  0x04

#define RCCU_DX_Mask   0x00000007

#define RCCU_FREEN_Mask    0x00000080

#define RCCU_CSU_CKSEL_Mask 0x00000001

#define RCCU_CK2_16_Mask    0x00000008

#define RCCU_CKAF_SEL_Mask  0x00000004

#define RCCU_LOCK_Mask     0x00000002

#define RCCU_USBEN_Mask   0x0100
#define RCCU_USBEN_Index  0x08
#define RCCU_ResetSources_Mask 0x000006E0

// RTC Oscillator Frequency value = 32 768 Hz
#define RCCU_RTC_Osc  32768

/*******************************************************************************
* Function Name  : RCCU_Div2Config
* Description    : Enables/Disables the clock division by two
* Input          : NewState : ENABLE or DISABLE
* Return         : None
*******************************************************************************/
static inline void RCCU_Div2Config ( FunctionalState NewState )
{
  if (NewState == ENABLE) RCCU->CFR |= RCCU_Div2_Mask;
  else RCCU->CFR &= ~RCCU_Div2_Mask;
}

/*******************************************************************************
* Function Name  : RCCU_Div2Status
* Description    : Gets the Div2 Flag status
* Input          : None
* Input          : FlagStatus
* Return         : None
*******************************************************************************/
static inline FlagStatus RCCU_Div2Status ( void )
{
  return (RCCU->CFR & RCCU_Div2_Mask) == 0 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : RCCU_MCLKConfig
* Description    : Selects the division factor for RCLK to obtain the
*                  MCLK clock for the CPU
* Input          : New_Clock : RCCU_DEFAULT, RCCU_RCLK_2, RCCU_RCLK_4, RCCU_RCLK_8
* Return         : None
*******************************************************************************/
static inline void RCCU_MCLKConfig ( RCCU_Clock_Div New_Clock )
{
  PCU->MDIVR = ( PCU->MDIVR & ~RCCU_FACT_Mask ) | New_Clock;
}

/*******************************************************************************
* Function Name  : RCCU_FCLKConfig
* Description    : Selects the division factor for RCLK to obtain the
*                  FCLK clock for the APB1 fast peripherals (PCLK1).
* Input          : New_Clock : RCCU_DEFAULT, RCCU_RCLK_2,
*                               RCCU_RCLK_4, RCCU_RCLK_8
* Return         : None
*******************************************************************************/
static inline void RCCU_FCLKConfig ( RCCU_Clock_Div New_Clock )
{
  PCU->PDIVR = ( PCU->PDIVR & ~RCCU_FACT1_Mask ) | New_Clock;
}

/*******************************************************************************
* Function Name  : RCCU_PCLKConfig
* Description    : Selects the division factor for RCLK to obtain the
*                  PCLK clock for the APB2 peripherals (PCLK2).
* Input          : New_Clock : RCCU_DEFAULT, RCCU_RCLK_2,
*                              RCCU_RCLK_4, RCCU_RCLK_8
* Return         : None
*******************************************************************************/
static inline void RCCU_PCLKConfig ( RCCU_Clock_Div New_Clock )
{
  PCU->PDIVR =(PCU->PDIVR & ~RCCU_FACT2_Mask)|(New_Clock << RCCU_FACT2_Index);
}

/*******************************************************************************
* Function Name  : RCCU_PLL1Config
* Description    : Configures the PLL1 div & mul factors.
* Input          : New_Mul : RCCU_Mul_12, RCCU_Mul_16, RCCU_Mul_20, RCCU_Mul_28
*                : New_Div : RCCU_Div_1, RCCU_Div_2, RCCU_Div_3,
*                            RCCU_Div_4, RCCU_Div_5, RCCU_Div_6, RCCU_Div_7
* Return         : None
*******************************************************************************/
void RCCU_PLL1Config ( RCCU_PLL1_Mul New_Mul, RCCU_PLL_Div New_Div );

/*******************************************************************************
* Function Name  : RCCU_PLL2Config
* Description    : Configures the PLL2 div & mul factors.
* Input          : New_Mul : RCCU_Mul_12, RCCU_Mul_16, RCCU_Mul_20, RCCU_Mul_28
*                : New_Div : RCCU_Div_1, RCCU_Div_2, RCCU_Div_3, RCCU_Div_4,
*                            RCCU_Div_5, RCCU_Div_6, RCCU_Div_7
* Return         : None
*******************************************************************************/
void RCCU_PLL2Config ( RCCU_PLL2_Mul New_Mul, RCCU_PLL_Div New_Div );

/*******************************************************************************
* Function Name  : RCCU_RCLKSourceConfig
* Description    : Selects the RCLK source clock
* Input          : New_Clock : RCCU_PLL1_OutPut, RCCU_CLOCK2_16, RCCU_CLOCK2
* Return         : None
*******************************************************************************/
void RCCU_RCLKSourceConfig ( RCCU_RCLK_Clocks New_Clock );

/*******************************************************************************
* Function Name  : RCCU_RCLKClockSource
* Description    : Returns the current RCLK source clock
* Input          : None
* Return         : RCCU_PLL1_Output, RCCU_CLOCK2_16, RCCU_CLOCK2
*******************************************************************************/
RCCU_RCLK_Clocks RCCU_RCLKClockSource ( void );

/*******************************************************************************
* Function Name  : RCCU_USBCLKConfig
* Description    : Selects the USB source clock
* Input          : New_Clock : RCCU_PLL2_Output, RCCU_USBCK
* Return         : None
*******************************************************************************/
static inline void RCCU_USBCLKConfig ( RCCU_USB_Clocks New_Clock )
{
  PCU->PLL2CR = (PCU->PLL2CR & ~RCCU_USBEN_Mask)|(New_Clock << RCCU_USBEN_Index);
}

/*******************************************************************************
* Function Name  : RCCU_USBClockSource
* Description    : Gets the USB source clock
* Input          : None
* Return         : RCCU_USB_Clocks
*******************************************************************************/
RCCU_USB_Clocks RCCU_USBClockSource ( void );

/*******************************************************************************
* Function Name  : RCCU_FrequencyValue
* Description    : Calculates & Returns any internal RCCU clock freuqency
*                  passed in parametres
* Input          : RCCU_Clocks : RCCU_CLK2, RCCU_RCLK, RCCU_MCLK,
*                  RCCU_PCLK, RCCU_FCLK
* Return         : u32
*******************************************************************************/
u32 RCCU_FrequencyValue ( RCCU_Clocks Internal_Clk );

/*******************************************************************************
* Function Name  : RCCU_ITConfig
* Description    : Configures the RCCU interrupts
* Input          : RCCU interrupts : RCCU_CK2_16_IT, RCCU_CKAF_IT,
*                  RCCU_PLL1_LOCK_IT
* Return         : None
*******************************************************************************/
static inline void RCCU_ITConfig ( RCCU_Interrupts RCCU_IT, FunctionalState NewState)
{
  if (NewState == ENABLE) RCCU->CCR |= RCCU_IT; else RCCU->CCR &= ~RCCU_IT;
}

/*******************************************************************************
* Function Name  : RCCU_FlagStatus
* Description    : Checks the RCCU clock flag register status
* Input          : RCCU Flags : RCCU_CK2_16, RCCU_CKAF, RCCU_PLL1_LOCK
* Return         : FlagStatus : SET or RESET
*******************************************************************************/
static inline FlagStatus RCCU_FlagStatus ( RCCU_Flags RCCU_flag )
{
  return (RCCU->CFR & RCCU_flag) == 0 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : RCCU_FlagClear
* Description    : Clears a specified flag in the RCCU registers
* Input          : RCCU Flags : RCCU_CK2_16, RCCU_CKAF, RCCU_PLL1_LOCK
* Return         : None
*******************************************************************************/
static inline void RCCU_FlagClear ( RCCU_Interrupts RCCU_IT )
{
  RCCU->CFR |= RCCU_IT;
}

/*******************************************************************************
* Function Name  : RCCU_ResetSources
* Description    : Return the source of the system reset
* Input          : None
* Return         : The reset source
*******************************************************************************/
static inline RCCU_ResetSources RCCU_ResetSource ( void )
{

  switch(RCCU->CFR & RCCU_ResetSources_Mask)
  {
    case 0x00000020: return RCCU_SoftwareReset;
    case 0x00000040: return RCCU_WDGReset;
    case 0x00000080: return RCCU_RTCAlarmReset;
    case 0x00000200: return RCCU_LVDReset;
    case 0x00000400: return RCCU_WKPReset;
    default : return RCCU_ExternalReset;
  }
}

//=========================================================================================================
//=========================================================================================================
// tim.h
//=========================================================================================================
//=========================================================================================================
typedef enum { TIM_EXTERNAL,
  TIM_INTERNAL
} TIM_Clocks; 

typedef enum { TIM_RISING,
  TIM_FALLING
} TIM_Clock_Edges; 

typedef enum { TIM_CHANNEL_A,
  TIM_CHANNEL_B
} TIM_Channels;  

typedef enum { TIM_WITH_IT,
  TIM_WITHOUT_IT
} TIM_IT_Mode;

typedef enum { TIM_TIMING,
  TIM_WAVE
} TIM_OC_Modes; 

typedef enum { TIM_HIGH,
  TIM_LOW
} TIM_Logic_Levels; 

typedef enum { TIM_START,
  TIM_STOP,
  TIM_CLEAR
} TIM_CounterOperations; 

typedef enum { TIM_ICFA = 0x8000,
  TIM_OCFA = 0x4000,
  TIM_TOF  = 0x2000,
  TIM_ICFB = 0x1000,
  TIM_OCFB = 0x0800
} TIM_Flags;

typedef struct { u16 Pulse;
  u16 Period;
} PWMI_parameters; 

#define TIM_ECKEN_Mask  0x0001 
#define TIM_EXEDG_Mask  0x0002 

#define TIM_IEDGA_Mask  0x0004 
#define TIM_IEDGB_Mask  0x0008 

#define TIM_PWM_Mask    0x0010 

#define TIM_OPM_Mask    0x0020 

#define TIM_OCAE_Mask   0x0040 
#define TIM_OCBE_Mask   0x0080  

#define TIM_OLVLA_Mask  0x0100 
#define TIM_OLVLB_Mask  0x0200  

#define TIM_FOLVA_Mask  0x0400  
#define TIM_FOLVB_Mask  0x0800 

#define TIM_PWMI_Mask   0x4000 

#define TIM_EN_Mask     0x8000  

#define TIM_OCBIE_mask  0x0800  
#define TIM_ICBIE_Mask  0x1000  
#define TIM_TOE_Mask    0x2000  
#define TIM_OCAIE_mask  0x4000   

#define TIM_ICAIE_Mask  0x8000   

#define TIM_ICA_IT   0x8000 /* Input Capture Channel A  */
#define TIM_OCA_IT   0x4000 /* Output Compare Channel A */
#define TIM_TO_IT    0x2000 /* Timer OverFlow           */
#define TIM_ICB_IT   0x1000 /* Input Capture Channel B  */
#define TIM_OCB_IT   0x0800 /* Input Capture Channel A  */

/*******************************************************************************
* Function Name  : TIM_Init
* Description    : This routine is used to Initialize the TIM peripheral
* Input          : TIM Timer to Initialize
* Return         : None
*******************************************************************************/
void TIM_Init( TIM_TypeDef *TIMx );

/*******************************************************************************
* Function Name  : TIM_ClockSourceConfig
* Description    : This routine is used to configure the TIM clock source
* Input          : (1) TIM Timer
*                : (2) TIM_Clocks : Specifies the TIM source clock
*                       - TIM_INTERNAL : The TIM is clocked by the APB2 frequency
*                                    divided by the prescaler value.
*                       - TIM_EXTERNAL : The TIM is clocked by an external Clock
* Return         : None
*******************************************************************************/
static inline void TIM_ClockSourceConfig ( TIM_TypeDef *TIMx, TIM_Clocks Xclock )
{
  if (Xclock==TIM_EXTERNAL) TIMx->CR1|=TIM_ECKEN_Mask; else TIMx->CR1&=~TIM_ECKEN_Mask;
}

/*******************************************************************************
* Function Name  : TIM_ClockSourceValue
* Description    : This routine is used to get the TIM clock source
* Input          : TIM Timer
* Return         : TIM_Clocks : Specifies the TIM source clock
*                       - TIM_INTERNAL : The TIM is clocked by the APB2 frequency
*                                    divided by the prescaler value.
*                       - TIM_EXTERNAL : The TIM is clocked by an external Clock
*******************************************************************************/
static inline TIM_Clocks TIM_ClockSourceValue ( TIM_TypeDef *TIMx )
{
  return ( TIMx->CR1 & TIM_ECKEN_Mask) == 0 ? TIM_INTERNAL : TIM_EXTERNAL;
}

/*******************************************************************************
* Function Name  : TIM_PrescalerConfig
* Description    : This routine is used to configure the TIM prescaler value
*                  ( using an internal clock )
* Input          : (1) TIM Timer
*                : (2) Prescaler ( u8 )
* Return         : None
*******************************************************************************/
static inline void TIM_PrescalerConfig ( TIM_TypeDef *TIMx, u8 Xprescaler )
{
  TIMx->CR2 = ( TIMx->CR2 & 0xFF00 ) | Xprescaler;
}

/*******************************************************************************
* Function Name  : TIM_PrescalerValue
* Description    : This routine is used to get the TIM prescaler value
*                  ( when using using an internal clock )
* Input          : TIM Timer
* Return         : Prescaler ( u8 )
*******************************************************************************/
static inline u8 TIM_PrescalerValue ( TIM_TypeDef *TIMx )
{
  return TIMx->CR2 & 0x00FF;
}

/*******************************************************************************
* Function Name  : TIM_ClockLevelConfig
* Description    : This routine is used to configure the TIM clock level
*                  ( using an external clock )
* Input          : TIM Timer
*                : TIM_Clock_Edges : Specifies the active adge of the external clock
*                  - TIM_RISING  : The rising  edge
*                  - TIM_FALLING : The falling edge
* Return         : None
*******************************************************************************/
static inline void TIM_ClockLevelConfig ( TIM_TypeDef *TIMx, TIM_Clock_Edges Xedge )
{
  if (Xedge == TIM_RISING) TIMx->CR1 |= TIM_EXEDG_Mask; else TIMx->CR1 &= ~TIM_EXEDG_Mask;
}

/*******************************************************************************
* Function Name  : TIM_ClockLevelValue
* Description    : This routine is used to get the TIM clock level
* Input          : TIM Timer
* Output         : TIM_Clock_Edges : Specifies the active adge of the external clock
*                  - TIM_RISING  : The rising  edge
*                  - TIM_FALLING : The falling edge
*******************************************************************************/
static inline TIM_Clock_Edges TIM_ClockLevelValue ( TIM_TypeDef *TIMx )
{
  return ( TIMx->CR1 & TIM_EXEDG_Mask ) == 0 ? TIM_FALLING : TIM_RISING;
}

/*******************************************************************************
* Function Name  : TIM_ICAPModeConfig
* Description    : This routine is used to configure the input capture feature
* Input          : (1) TIM Timer
*                : (2) Input Capture Channel ( Channel_A or Channel_B )
*                : (3) Active Edge : Rising edge or Falling edge.
* Output         : None
*******************************************************************************/
void TIM_ICAPModeConfig ( TIM_TypeDef  *TIMx,
                          TIM_Channels Xchannel,
                          TIM_Clock_Edges  Xedge );

/*******************************************************************************
* Function Name  : TIM_ICAPValue
* Description    : This routine is used to get the Input Capture value
* Input          : (1) TIM Timer
*                : (2) Input Capture Channel ( Channel_A or Channel_B )
* Output         : None
*******************************************************************************/
static inline u16 TIM_ICAPValue ( TIM_TypeDef *TIMx, TIM_Channels Xchannel )
{
  return Xchannel == TIM_CHANNEL_A ? TIMx->ICAR : TIMx->ICBR;
}

/*******************************************************************************
* Function Name  : TIM_OCMPModeConfig
* Description    : This routine is used to configure the output compare feature
* Input          : (1) TIM Timer
*                : (2) OCMP Channel ( Channel_A or Channel_B )
*                : (3) Pulse Length
*                : (4) OC_Mode     : output wave, or only timing.
*                : (5) Level       : Rising edge or Falling edge after the ==
* Output         : None
*******************************************************************************/
void TIM_OCMPModeConfig (  TIM_TypeDef  *TIMx,
                           TIM_Channels Xchannel,
                           u16          XpulseLength,
                           TIM_OC_Modes     Xmode,
                           TIM_Logic_Levels Xlevel );

/*******************************************************************************
* Function Name  : TIM_OPModeConfig
* Description    : This routine is used to configure the one pulse mode
* Input          : (1) TIM Timer
*                : (3) XpulseLength      : Length of the pulse
*                : (4) Level1            : Level during the pulse
*                : (5) Level2            : Level after the pulse
*                : (6) Activation Edge   : High or Low on ICAP A
* Output         : None
*******************************************************************************/
void TIM_OPModeConfig ( TIM_TypeDef  *TIMx,
                        u16          XpulseLength,
                        TIM_Logic_Levels XLevel1,
                        TIM_Logic_Levels XLevel2,
                        TIM_Clock_Edges  Xedge );

/*******************************************************************************
* Function Name  : TIM_PWMOModeConfig
* Description    : This routine is used to configure the PWM in output mode
* Input          : (1) TIM Timer
*                : (2) DutyCycle   : u16
*                : (3) Level 1     : During the Duty Cycle
*                : (4) Level 2     : During the after the pulse
*                : (5) Full period : u16
* Output         : None
*******************************************************************************/
void TIM_PWMOModeConfig ( TIM_TypeDef  *TIMx,
                          u16          XDutyCycle,
                          TIM_Logic_Levels XLevel1,
                          u16          XFullperiod,
                          TIM_Logic_Levels XLevel2
                        );

/*******************************************************************************
* Function Name  : TIM_PWMInputConfig
* Description    : This routine is used to configure the PWM in input mode
* Input          : (1) TIM Timer
*                : (2) First Activation Edge
* Output         : None
*******************************************************************************/
void TIM_PWMIModeConfig ( TIM_TypeDef *TIMx, TIM_Clock_Edges Xedge );

/*******************************************************************************
* Function Name  : TIM_PWMIValue
* Description    : This routine is used to get the PWMI values
* Input          : (1) TIM Timer
* Output         : PWMI_parameters : - u16 Dyty cycle
                                     - u16 Full period
*******************************************************************************/
PWMI_parameters TIM_PWMIValue (TIM_TypeDef *TIMx );

/*******************************************************************************
* Function Name  : TIM_CounterConfig
* Description    : This routine is used to start/stop and clear the selected
*                  timer counter
* Input          : (1) TIM Timer
*                : (2) TIM_CounterOperations
                      TIM_START Enables or resumes the counter
*                     TIM_STOP  Stops the TIM counter
*                     TIM_CLEAR Set the TIM counter value to FFFCh
* Output         : None
*******************************************************************************/
void TIM_CounterConfig ( TIM_TypeDef *TIMx, TIM_CounterOperations Xoperation );

/*******************************************************************************
* Function Name  : TIM_ITConfig
* Description    : This routine is used to configure the TIM IT
* Input          : (1) TIM Timer
*                : (2) TIM interrupt (input capture A or input capture B or timer overflow or out compA or outpu com B
*                : (2) ENABLE / DISABLE
* Output         : None
*******************************************************************************/
static inline void TIM_ITConfig ( TIM_TypeDef *TIMx, u16 New_IT, FunctionalState NewState )
{
  if (NewState == ENABLE) TIMx->CR2 |= New_IT; else TIMx->CR2 &= ~New_IT;
}

/*******************************************************************************
* Function Name  : TIM_FlagStatus
* Description    : This routine is used to check whether a Flag is Set.
* Input          : (1) TIM Timer
*                : (2) The TIM FLag
* Output         : Flag NewState
*******************************************************************************/
static inline FlagStatus TIM_FlagStatus ( TIM_TypeDef *TIMx, TIM_Flags Xflag )
{
  return (TIMx->SR & Xflag) == 0 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : TIM_FlagClear
* Description    : This routine is used to clear Flags.
* Input          : (1) TIM Timer
*                : (2) The TIM FLag
* Output         : None
*******************************************************************************/
static inline void TIM_FlagClear ( TIM_TypeDef *TIMx, TIM_Flags Xflag )
{
  TIMx->SR &= ~Xflag;
}

/*******************************************************************************
* Function Name  : TIM_CounterValue
* Description    : This routine returns the timer counter value.
* Input          : TIM Timer
* Output         : The counter value
*******************************************************************************/
static inline u16 TIM_CounterValue(TIM_TypeDef *TIMx)
{
  return TIMx->CNTR;
}

//=========================================================================================================
//=========================================================================================================
// uart.h
//=========================================================================================================
//=========================================================================================================
typedef enum
{
  UART_RxFIFO,
  UART_TxFIFO
} UARTFIFO_TypeDef;

typedef enum
{
  UART_EVEN_PARITY = 0x0000,
  UART_ODD_PARITY  = 0x0020,
  UART_NO_PARITY  = 0x0000 
} UARTParity_TypeDef;

typedef enum
{
  UART_0_5_StopBits  = 0x00,
  UART_1_StopBits    = 0x08,
  UART_1_5_StopBits  = 0x10,
  UART_2_StopBits    = 0x18
} UARTStopBits_TypeDef;

typedef enum
{
  UARTM_8D   = 0x01,
  UARTM_7D_P = 0x03,
  UARTM_9D   = 0x04,
  UARTM_8D_W = 0x05,
  UARTM_8D_P = 0x07
} UARTMode_TypeDef;

/* UART flags definition */
#define UART_TxFull          0x0200
#define UART_RxHalfFull      0x0100
#define UART_TimeOutIdle     0x0080
#define UART_TimeOutNotEmpty 0x0040
#define UART_OverrunError    0x0020
#define UART_FrameError      0x0010
#define UART_ParityError     0x0008
#define UART_TxHalfEmpty     0x0004
#define UART_TxEmpty         0x0002
#define UART_RxBufFull       0x0001

/*******************************************************************************
* Function Name  : UART_Init
* Description    : This function initializes the selected UART registers to 
*                  their reset values
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Init(UART_TypeDef *UARTx);

/*******************************************************************************
* Function Name  : UART_ModeConfig
* Description    : This function configures the mode of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The UART mode, it can be
*                  UARTM_8D 	for 8-bit data format
*                  UARTM_7D_P 	for 7-bit data + parity format
*                  UART_9D 	for 9-bit data format
*                  UART_8D_W	for 8-bit data + wake-up bit format
*                  UART_8D_P	for 8-bit data + parity bit format 
* Output         : None
* Return         : None
*******************************************************************************/
static inline void UART_ModeConfig(UART_TypeDef *UARTx, UARTMode_TypeDef UART_Mode)
{
  UARTx->CR = (UARTx->CR&0xFFF8)|(u16)UART_Mode;
}

/*******************************************************************************
* Function Name  : UART_BaudRateConfig
* Description    : This function configures the baud rate of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The baudrate value
* Output         : None
* Return         : None
*******************************************************************************/
void UART_BaudRateConfig(UART_TypeDef *UARTx, u32 BaudRate);

/*******************************************************************************
* Function Name  : UART_ParityConfig
* Description    : This function configures the data parity of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the desired UART
* Input 2        : The parity type, it can be UART_EVEN_PARITY, UART_ODD_PARITY
*                  or UART_NO_PARITY
* Output         : None
* Return         : None
*******************************************************************************/
static inline void UART_ParityConfig(UART_TypeDef *UARTx, UARTParity_TypeDef Parity)
{
  UARTx->CR = (UARTx->CR&0xFFDF)|(u16)Parity;
}

/*******************************************************************************
* Function Name  : UART_StopBitsConfig
* Description    : This function configures the number of stop bits of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The number of stop bits, it can be
*                  UART_0_5_StopBits	for 0.5 stop bit
*                  UART_1_StopBits	for 1 stop bit, 
*                  UART_1_5_StopBits 	for 1.5 stop bits
*	           UART_2_StopBits	for 2 stop bits
* Output         : None
* Return         : None
*******************************************************************************/
static inline void UART_StopBitsConfig(UART_TypeDef *UARTx, UARTStopBits_TypeDef StopBits)
{
  UARTx->CR = (UARTx->CR&0xFFE7)|(u16)StopBits;
}

/*******************************************************************************
* Function Name  : UART_Config
* Description    : This function configures the baudrate, the mode, the data
*                  parity and the number of stop bits of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : u32 The baudrate value
* Input 3        : The parity type, it can be UART_EVEN_PARITY, UART_ODD_PARITY
*		   or UART_NO_PARITY
* Input 4        : The number of stop bits UART_0_5_StopBits, UART_1_StopBits, 
*                  UART_1_5_StopBits or UART_2_StopBits
* Input 5        : The UART mode, it can be
*                  UARTM_8D 	for 8-bit data format
*                  UARTM_7D_P 	for 7-bit data + parity format
*                  UART_9D 	for 9-bit data format
*                  UART_8D_W	for 8-bit data + wake-up bit format
*                  UART_8D_P	for 8-bit data + parity bit format   
* Output         : None
* Return         : None
*******************************************************************************/
void UART_Config(UART_TypeDef *UARTx, u32 BaudRate, UARTParity_TypeDef Parity,
                 UARTStopBits_TypeDef StopBits, UARTMode_TypeDef Mode);

/*******************************************************************************
* Function Name  : UART_ItConfig
* Description    : This function enables or disables one or several interrupt
*                  sources of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The new interrupt flag or flags  
* Input 3        : ENABLE or DISABLE
* Output         : None
* Return         : None
* Note           : The UART interrupt flags are listed in the file uart.h
*******************************************************************************/
void UART_ItConfig(UART_TypeDef *UARTx, u16 UART_Flag, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_FifoConfig
* Description    : This function enables or disables the Rx and Tx FIFOs of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_FifoReset
* Description    : This function resets the Rx and the Tx FIFOs of the
*                  selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : RxFIFO or TxFIFO
* Output         : None
* Return         : None
*******************************************************************************/
void UART_FifoReset(UART_TypeDef *UARTx, UARTFIFO_TypeDef FIFO);

/*******************************************************************************
* Function Name  : UART_LoopBackConfig
* Description    : This function enables or disables the loop back mode of
*                  the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_LoopBackConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_TimeOutPeriodConfig
* Description    : This function configures the UART Time Out Period.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The time-out period value
* Output         : None
* Return         : None
*******************************************************************************/
static inline void UART_TimeOutPeriodConfig(UART_TypeDef *UARTx, u16 TimeOutPeriod)
{
  UARTx->TOR = TimeOutPeriod;
}

/*******************************************************************************
* Function Name  : UART_GuardTimeConfig
* Description    : This function configures the UART Guard Time.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : The guard time value
* Output         : None
* Return         : None
*******************************************************************************/
static inline void UART_GuardTimeConfig(UART_TypeDef *UARTx, u16 GuardTime)
{
  UARTx->GTR = GuardTime;
}

/*******************************************************************************
* Function Name  : UART_RxConfig
* Description    : This function enable and disable the UART data reception.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_RxConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_OnOffConfig
* Description    : This function sets On/Off the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : ENABLE or DISABLE
* Output         : None
* Return         : None
*******************************************************************************/
void UART_OnOffConfig(UART_TypeDef *UARTx, FunctionalState NewState);

/*******************************************************************************
* Function Name  : UART_ByteSend
* Description    : This function sends a byte to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the data byte to send
* Output         : None
* Return         : None
*******************************************************************************/
void UART_ByteSend(UART_TypeDef *UARTx, u8 *Data);

/*******************************************************************************
* Function Name  : UART_9BitByteSend
* Description    : This function sends a 9-bit data to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the data to send
* Output         : None
* Return         : None
*******************************************************************************/
void UART_9BitByteSend(UART_TypeDef *UARTx, u16 *Data);

/*******************************************************************************
* Function Name  : UART_DataSend
* Description    : This function sends several data bytes to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the data to send
* Input 3        : The data length in bytes
* Output         : None
* Return         : None
*******************************************************************************/
void UART_DataSend(UART_TypeDef *UARTx, u8 *Data, u8 DataLength);

/*******************************************************************************
* Function Name  : UART_9BitDataSend
* Description    : This function sends several 9-bits data to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the data to send
* Input 3        : The data length
* Output         : None
* Return         : None
*******************************************************************************/
void UART_9BitDataSend(UART_TypeDef *UARTx, u16 *Data, u8 DataLength);

/*******************************************************************************
* Function Name  : UART_StringSend
* Description    : This function sends a string to the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the string to send
* Output         : None
* Return         : None
*******************************************************************************/
void UART_StringSend(UART_TypeDef *UARTx, u8 *String);

/*******************************************************************************
* Function Name  : UART_ByteReceive
* Description    : This function gets a data byte from the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the buffer where the data will be stored
* Input 3        : The time-out period
* Output         : The received 8-bit data
* Return         : The UARTx.SR register content
*******************************************************************************/
u16 UART_ByteReceive(UART_TypeDef *UARTx, u8 *Data, u8 TimeOut);

/*******************************************************************************
* Function Name  : UART_9BitByteReceive
* Description    : This function gets a 9-bit data byte from the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the buffer where the data will be stored
* Input 3        : The time-out period value
* Output         : The received 9-bit data
* Return         : The UARTx.SR register content
*******************************************************************************/
u16 UART_9BitByteReceive(UART_TypeDef *UARTx, u16 *Data, u8 TimeOut);

/*******************************************************************************
* Function Name  : UART_DataReceive
* Description    : This function gets 8 bits data bytes from the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer to the buffer where the data will be stored
* Input 3        : The data length
* Input 4        : The time-out period value
* Output         : The received 8-bit data buffer
* Return         : The UARTx.SR register content
*******************************************************************************/
u16 UART_DataReceive(UART_TypeDef *UARTx, u8 *Data, u8 DataLength, u8 TimeOut);

/*******************************************************************************
* Function Name  : UART_9BitDataReceive
* Description    : This function gets 9 bits data bytes from the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer to the buffer where the data will be stored
* Input 3        : The data length
* Input 4        : The time-out value
* Output         : The received 9-bit data buffer
* Return         : The UARTx.SR register content
*******************************************************************************/
u16 UART_9BitDataReceive(UART_TypeDef *UARTx, u16 *Data, u8 DataLength, u8 TimeOut);

/*******************************************************************************
* Function Name  : UART_StringReceive
* Description    : This function gets 8-bit data bytes from the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Input 2        : A pointer on the buffer where the string will be stored
* Output         : None
* Return         : None
*******************************************************************************/
u16 UART_StringReceive(UART_TypeDef *UARTx, u8 *Data);

/*******************************************************************************
* Function Name  : UART_FlagStatus
* Description    : This function gets the flags status of the selected UART.
* Input 1        : UARTx (x can be 0,1, 2 or 3) the selected UART
* Output         : None
* Return         : None
*******************************************************************************/
static inline u16 UART_FlagStatus(UART_TypeDef *UARTx)
{
  return UARTx->SR;
}

//=========================================================================================================
//=========================================================================================================
// bspi.h
//=========================================================================================================
//=========================================================================================================

typedef enum {
  BSPI_BERIT = 0x80,
  BSPI_RCIT  = 0x10,
  BSPI_ALL   = 0x90
} BSPI_ITS;

typedef enum {
  BSPI_BERR = 0x004,
  BSPI_RFNE = 0x008,
  BSPI_RFF  = 0x010,
  BSPI_ROFL = 0x020,
  BSPI_TFE  = 0x040,
  BSPI_TUFL = 0x080,
  BSPI_TFF  = 0x100,
  BSPI_TFNE = 0x200
} BSPI_Flags;

typedef enum {
  BSPI_TR_FE,
  BSPI_TR_UFL,
  BSPI_TR_FF,
  BSPI_TR_DIS
} BSPI_TR_IT_SRCS;

typedef enum {
  BSPI_RC_FNE,
  BSPI_RC_FF,
  BSPI_RC_DIS
} BSPI_RC_IR_SRCS;

#define BSPI_PESET_Mask 0x01
#define BSPI_DFIFO_Mask 0xFFFE
#define BSPI_CPHA_Mask  0x200
#define BSPI_CPOL_Mask  0x100
#define BSPI_BSPI0_Base 0xA000
#define BSPI_BSPI1_Base 0xB000


/*******************************************************************************
* Function Name  : BSPI_BSPI0Conf
* Description    : configure STR71x on BSPI0 mode.
* Input 2        : NewState: specifies the status of the BSPI
*		 : NewState value must be either ENABLE or DISABLE:
*		 : ENABLE: the specified BSPI0 peripheral will be enabled.
*		 : DISABLE: the specified BSPI0 peripheral will be disabled..
* Output         : SPI0 EN  bit  in BOOTCR.
* Return         : None.
*******************************************************************************/
static inline void BSPI_BSPI0Conf(FunctionalState NewState)
{
  if (NewState == ENABLE) PCU->BOOTCR |= 0x04; else PCU->BOOTCR &= ~0x04;
}

/*******************************************************************************
* Function Name  : BSPI_Init
* Description    : Initializes BSPI peripheral control and registers to their default reset values.
* Input          : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : BSPI peripheral registers and BSPI global variables are initialized.
* Return         : None.
*******************************************************************************/
void BSPI_Init( BSPI_TypeDef *BSPIx);

/*******************************************************************************
* Function Name  : BSPI_Enable
* Description    : Enables/disables the specified BSPI peripheral.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral to be enabled or disabled.
* Input 2        : NewState: specifies the status of the BSPI
*		 : NewState value must be either ENABLE or DISABLE:
*		 : ENABLE: the specified BSPI peripheral will be enabled.
*		 : DISABLE: the specified BSPI peripheral will be disabled.
* Output         : BSPE bit  in BSPCSR1.
* Return         : None.
*******************************************************************************/
static inline void BSPI_Enable( BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_PESET_Mask;
  else BSPIx->CSR1 &= ~BSPI_PESET_Mask;
}

/*******************************************************************************
* Function Name  : BSPI_MasterEnable
* Description    : Configures the BSPI as a Master or a Slave.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : NewState: specifies whether configuring BSPI as master is enabled or disabled.
*		 : ENABLE: the specified BSPI will be configured as a master.
*		 : DISABLE: the specified BSPI will be configured as a slave.
* Output         : MSTR bit in BSPCSR1 is modified register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_MasterEnable(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= 0x02; else BSPIx->CSR1 &= ~0x02;
}

/*******************************************************************************
* Function Name  : BSPI_TrItSrc.
* Description    : Configures the transmit interrupt source.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : TrItSrc: specifies the transmit interrupt source.
*		 : Refer to the section ‘Transmit Interrupt Sources’ for more details on the
*		 : allowed values of this parameter.
* Output         : TIE bit in BSPCSR2 register.
* Return         : None.
*******************************************************************************/
void BSPI_TrItSrc(BSPI_TypeDef *BSPIx, BSPI_TR_IT_SRCS TrItSrc);

/*******************************************************************************
* Function Name  : BSPI_RcItSrc.
* Description    : Configures the receive interrupt source.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : RcItSrc: specifies the source for the receive interrupt.
*		 : Refer to the section ‘Receive Interrupt Sources’ for more details on the
*		 : allowed values of this parameter.
* Output         : RIE bit in BSPCSR1 is register.
* Return         : None.
*******************************************************************************/
void BSPI_RcItSrc(BSPI_TypeDef *BSPIx, BSPI_RC_IR_SRCS RcItSrc);

/*******************************************************************************
* Function Name  : BSPI_TrFifoDepth
* Description    : Configures BSPI transmission FIFO number of words.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI.
* Input 2        : TDepth:specifies the depth of the transmit FIFO.
* Output         : TFE bit in BSPCSR2 register.
* Return         : None.
*******************************************************************************/
void BSPI_TrFifoDepth(BSPI_TypeDef *BSPIx, u8 TDepth);

/*******************************************************************************
* Function Name  : BSPI_RcFifoDepth
* Description    : Configures BSPI reception FIFO number of words.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : RDepth:specifies the depth of the receive FIFO.
* Output         : RFE bits [3:0] in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
void BSPI_RcFifoDepth(BSPI_TypeDef *BSPIx, u8 RDepth);

/*******************************************************************************
* Function Name  : BSPI_8bLEn
* Description    : Sets the word length of the receive FIFO and transmit data registers to either 8 or 16 bits.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI.
* Input 2        : status: specifies if the word length is 8 or 16 bits.
*                : status value must be either ENABLE or DISABLE:
*                : ENABLE: to enable setting the word length to 8 bits.
*                : DISABLE: disables setting the word length to 8 bits: the wordlength will be configured to 16 bits.
* Output         : WL bits in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_8bLEn(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == DISABLE) BSPIx->CSR1 |= 0x0400; else BSPIx->CSR1 &= ~0x0C00;
}

/*******************************************************************************
* Function Name  : BSPI_ClkFEdge
* Description    : Enables capturing the first data sample on the first edge of SCK or on the second edge.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : NewState: specifies whether capturing the first data sample on the first edge of SCK is enabled or disabled.
*		 : status value must be either ENABLE or DISABLE.
* 		 :DISABLE: to enable capturing the first data sample on the first edge of SCK.
* 		 : ENABLE: to enable capturing the first data sample on the second edge of SCK.
* Output         : CPHA bits in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_ClkFEdge(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_CPHA_Mask;
  else BSPIx->CSR1&=~BSPI_CPHA_Mask;
}

/*******************************************************************************
* Function Name  : BSPI_ClkActiveHigh
* Description    : Configures the clock to be active high or low.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : NewState: specifies whether the clock is active high or low.
*		 : status value must be ENABLE or DISABLE.
*		 : ENABLE: configures the clock to be active high.
*		 : DISABLE: confgures the clock to be active low.
* Output         : CPOL bits in BSPCSR1 register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_ClkActiveHigh(BSPI_TypeDef *BSPIx, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_CPOL_Mask;
  else BSPIx->CSR1 &= ~BSPI_CPOL_Mask;
}

/*******************************************************************************
* Function Name  : BSPI_FifoDisable
* Description    : Configure the BSPI FIFO.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : DFIFO bit in BSPCSR2 register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_FifoDisable(BSPI_TypeDef *BSPIx)
{
  BSPIx->CSR2 |= 0x0001;
}

/*******************************************************************************
* Function Name  : BSPI_ClockDividerConfig
* Description    : Configure BSPI clock divider
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI.
* Input 2        : Div: holds the value of the clock divider.
* Output         : Div [7:0] bits in BSPCLK register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_ClockDividerConfig(BSPI_TypeDef *BSPIx, u8 Div)
{
  BSPIx->CLK = Div;
}

/*******************************************************************************
* Function Name  : BSPI_FlagStatus.
* Description    : Check whether the specified BSPI Flag is set or not.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : flag: specifies the flag to see the status.
*                : Refer to the section ‘BSPI Flags’ for more details on the allowed values of this parameter.
* Output         : None.
* Return         : FlagStatus type
*		 : SET: if the tested flag is set.
*                 : RESET: if the corresponding flag is reset.
*******************************************************************************/
static inline FlagStatus BSPI_FlagStatus(BSPI_TypeDef *BSPIx, BSPI_Flags flag)
{
  return (BSPIx->CSR2 & flag) != 0 ? SET : RESET;
}

/*******************************************************************************
* Function Name  : BSPI_WordSend.
* Description    : Transmit a single Word.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : holds the word which will be transmitted.
* Output         : BSPTXR register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_WordSend(BSPI_TypeDef *BSPIx, u16 Data)
{
  if ((BSPIx->CSR1 & 0x0400) == 0) Data <<= 8;
  BSPIx->TXR = Data;
}

/*******************************************************************************
* Function Name  : BSPI_BufferSend.
* Description    : Transmits data from a buffer.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : PtrToBuffer is an ‘u8’ pointer to the first word of the buffer to be transmitted.
* Input 3        : NbOfWords parameter indicates the number of words saved in the buffer to be sent.
* Output         : None.
* Return         : None.
*******************************************************************************/
void BSPI_BufferSend(BSPI_TypeDef *BSPIx, u8 *PtrToBuffer, u8 NbOfWords);

/*******************************************************************************
* Function Name  : BSPI_WordReceive.
* Description    : Returns the recent received word.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Output         : None.
* Return         : The value of the received word.
*******************************************************************************/
static inline u16 BSPI_WordReceive(BSPI_TypeDef *BSPIx)
{
  return (BSPIx->CSR1 & 0x0400) == 0 ? BSPIx->RXR >> 8 : BSPIx->RXR;
}

/*******************************************************************************
* Function Name  : BSPI_BufferReceive.
* Description    : Receives number of data words and stores them in user defined area.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : PtrToBuffer is an ‘u8’ pointer to the first word of the defined area to save the received buffer.
* Input 3        : NbOfWords parameter indicates the number of words to be received in the buffer.
* Output         : None.
* Return         : None.
*******************************************************************************/
void BSPI_BufferReceive(BSPI_TypeDef *BSPIx, u8 *PtrToBuffer, u8 NbOfWords);

/*******************************************************************************
* Function Name  : BSPI_ItEnable.
* Description    : Enable the specified interrupt.
* Input 1        : BSPIx where x can be 0 or 1 to select the BSPI peripheral.
* Input 2        : BSPI_IT: specifies the BSPI interrupt.
*                : Refer to the section ‘Interrupt Enable’ for more details on the allowed values of this parameter..
* Input 3        : NewState: specified whether the BSPI interrupt is enabled or disabled.
*		 : status value must be either ENABLE or DISABLE.
*		 : ENABLE: to enable interrupt
*		 : DISABLE: to disable interrupt.
* Output         : BSPCSR1 register.
* Return         : None.
*******************************************************************************/
static inline void BSPI_ItEnable(BSPI_TypeDef *BSPIx, BSPI_ITS BSPI_IE, FunctionalState NewState)
{
  if (NewState == ENABLE) BSPIx->CSR1 |= BSPI_IE; else BSPIx->CSR1 &= ~BSPI_IE;
}

#endif
#endif
