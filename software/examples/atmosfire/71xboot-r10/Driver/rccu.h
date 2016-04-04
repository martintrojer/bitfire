/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : rccu.h
* Author             : MCD Application Team
* Date First Issued  : 28/07/2003
* Description        : This file contains all the functions prototypes for the
*                      RCCU software library.
********************************************************************************
* History:
*  03/07/03 : First Version
*******************************************************************************/

#ifndef __RCCU_H
#define __RCCU_H

#include "71x_map.h"

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
               RCCU_Mul_12=0x01,
               RCCU_Mul_16=0x03,
               RCCU_Mul_20=0x00,
               RCCU_Mul_28=0x02
             } RCCU_PLL_Mul;

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
               RCCU_PLL2_Output=0x01,
               RCCU_USBCK=0x00
             } RCCU_USB_Clocks;

typedef enum {
               RCCU_CLK2,
               RCCU_RCLK,
               RCCU_MCLK,
               RCCU_PCLK,
               RCCU_FCLK
             } RCCU_Clocks;

typedef enum {
               RCCU_CK2_16_IT    = 0x0200,
               RCCU_CKAF_IT      = 0x0100,
               RCCU_PLL1_LOCK_IT = 0x0080
             } RCCU_Interrupts;

typedef enum {
               RCCU_CK2_16    = 0x2000,
               RCCU_CKAF      = 0x1000,
               RCCU_PLL1_LOCK = 0x0800
             } RCCU_Flags;

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


// Main Oscillator Frequency value = 16 Mhz
#define RCCU_Main_Osc 16000000
// RTC Oscillator Frequency value = 32 768 Hz
#define RCCU_RTC_Osc  32768


/*******************************************************************************
* Function Name  : RCCU_Div2Config
* Description    : Enables/Disables the clock division by two
* Input          : New_Status ( ENABLE or DISABLE )
* Return         : None
*******************************************************************************/
inline void RCCU_Div2Config ( functionalstate New_Status )
{
	if (New_Status == ENABLE) RCCU->CLK_FLAG |= RCCU_Div2_Mask; else RCCU->CLK_FLAG &= ~RCCU_Div2_Mask;
}

/*******************************************************************************
* Function Name  : RCCU_Div2Status
* Description    : Gets the Div2 Flag status
* Input          : None
* Input          : flagstatus
* Return         : None
*******************************************************************************/
inline flagstatus RCCU_Div2Status ( void )
{
	return (RCCU->CLK_FLAG & RCCU_Div2_Mask) == 0 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : RCCU_MCLKConfig
* Description    : Selects the division factor for RCLK to obtain the
*                  MCLK clock for the CPU
* Input          : New_Clock ( RCCU_DEFAULT, RCCU_RCLK_2, RCCU_RCLK_4, RCCU_RCLK_8 )
* Return         : None
*******************************************************************************/
inline void RCCU_MCLKConfig ( RCCU_Clock_Div New_Clock )
{
	PCU->CPUDIV = ( PCU->CPUDIV & ~RCCU_FACT_Mask ) | New_Clock;
}

/*******************************************************************************
* Function Name  : RCCU_FCLKConfig
* Description    : Selects the division factor for RCLK to obtain the
*                  FCLK clock for the fast peripherals.
* Input          : New_Clock ( RCCU_DEFAULT, RCCU_RCLK_2, RCCU_RCLK_4, RCCU_RCLK_8 )
* Return         : None
*******************************************************************************/
inline void RCCU_FCLKConfig ( RCCU_Clock_Div New_Clock )
{
	PCU->APBDIV = ( PCU->APBDIV & ~RCCU_FACT1_Mask ) | New_Clock;
}

/*******************************************************************************
* Function Name  : RCCU_PCLKConfig
* Description    : Selects the division factor for RCLK to obtain the
*                  FCLK clock for the APB1 Peripheral.
* Input          : New_Clock ( RCCU_DEFAULT, RCCU_RCLK_2, RCCU_RCLK_4, RCCU_RCLK_8 )
* Return         : None
*******************************************************************************/
inline void RCCU_PCLKConfig ( RCCU_Clock_Div New_Clock )
{
	PCU->APBDIV = ( PCU->APBDIV & ~RCCU_FACT2_Mask ) | ( New_Clock << RCCU_FACT2_Index );
}

/*******************************************************************************
* Function Name  : RCCU_PLL1Config
* Description    : Configures the PLL1 div & mul factors.
* Input          : New_Mul ( RCCU_Mul_12, RCCU_Mul_16, RCCU_Mul_20, RCCU_Mul_28 )
*                : New_Div ( RCCU_Div_1, RCCU_Div_2, RCCU_Div_3, RCCU_Div_4, RCCU_Div_5, RCCU_Div_6, RCCU_Div_7)
* Return         : None
*******************************************************************************/
void RCCU_PLL1Config ( RCCU_PLL_Mul New_Mul, RCCU_PLL_Div New_Div );

/*******************************************************************************
* Function Name  : RCCU_PLL2Config
* Description    : Configures the PLL2 div & mul factors.
* Input          : New_Mul ( RCCU_Mul_12, RCCU_Mul_16, RCCU_Mul_20, RCCU_Mul_28 )
*                : New_Div ( RCCU_Div_1, RCCU_Div_2, RCCU_Div_3, RCCU_Div_4, RCCU_Div_5, RCCU_Div_6, RCCU_Div_7)
* Return         : None
*******************************************************************************/
void RCCU_PLL2Config ( RCCU_PLL_Mul New_Mul, RCCU_PLL_Div New_Div );

/*******************************************************************************
* Function Name  : RCCU_RCLKSourceConfig
* Description    : Selects the RCLK source clock
* Input          : New_Clock ( RCCU_PLL1_OutPut, RCCU_CLOCK2_16, RCCU_CLOCK2 )
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
* Input          : New_Clock ( RCCU_PLL2_Output, RCCU_USBCK )
* Return         : None
*******************************************************************************/
inline void RCCU_USBCLKConfig ( RCCU_USB_Clocks New_Clock )
{
	PCU->PLL2CTL = ( PCU->PLL2CTL & ~RCCU_USBEN_Mask ) | ( New_Clock << RCCU_USBEN_Index );
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
* Input          : RCCU_Clocks ( RCCU_CLK2, RCCU_RCLK, RCCU_MCLK, RCCU_PCLK, RCCU_FCLK )
* Return         : u32
*******************************************************************************/
u32 RCCU_FrequencyValue ( RCCU_Clocks Internal_Clk );

/*******************************************************************************
* Function Name  : RCCU_ITConfig
* Description    : Configures the RCCU interrupts
* Input          : RCCU interrupts RCCU_CK2_16_IT, RCCU_CKAF_IT, RCCU_PLL1_LOCK_IT
* Return         : None
*******************************************************************************/
inline void RCCU_ITConfig ( RCCU_Interrupts RCCU_IT, functionalstate NewStatus)
{
  if (NewStatus == ENABLE) RCCU->CLKCTL |= RCCU_IT; else RCCU->CLKCTL &= ~RCCU_IT;
}

/*******************************************************************************
* Function Name  : RCCU_ITStatus
* Description    : Checks the RCCU interrupts status
* Input          : RCCU interrupts : RCCU_CK2_16_IT, RCCU_CKAF_IT, RCCU_PLL1_LOCK_IT
* Return         : flagstatus : SET or RESET
*******************************************************************************/
inline flagstatus RCCU_ITStatus ( RCCU_Interrupts RCCU_IT )
{
  return (RCCU->CLKCTL & RCCU_IT) == 0 ? RESET : SET;
}

/*******************************************************************************
* Function Name  : RCCU_FlagClear
* Description    : Clears a specified flag in the RCCU registers
* Input          : RCCU Flags : RCCU_CK2_16, RCCU_CKAF, RCCU_PLL1_LOCK
* Return         : None
*******************************************************************************/
inline void RCCU_FlagClear ( RCCU_Flags RCCU_flag )
{
	RCCU->CLK_FLAG |= RCCU_flag;
}

#endif	// __RCCU_H

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
