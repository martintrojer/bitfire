/******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
* File Name          : eic.c
* Author             : MCD Application Team
* Date First Issued  : 25/06/2003
* Description        : This file provides all the EIC software functions
********************************************************************************
* History:
*  25/06/2003 : Created
*******************************************************************************/

#include "eic.h"

/*******************************************************************************
* Function Name  : EIC_Init
* Description    : Initialise the EIC using the load PC instruction
*                  (PC = PC +offset)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EIC_Init(void)
{
  extern T0TIMI_Addr;
  u8 bCounter;
  u32 dOffset;
  EIC->ICR = 0x00000000; // Disable FIQ and IRQ interrupts
  EIC->IER = 0x00000000; // Disable all channels interrupts
  EIC->IPR = 0xFFFFFFFF; // Clear all pending bits
  EIC->FIR = 0x0000000C; // Disable all FIQ channels interrupts and clear FIQ
                         // channels pending bits
  EIC->CIPR = 0x00000000; // Ste the current priority level to zero
  EIC->IVR  = 0xE59F0000; // IVR = high half of load PC instruction
  dOffset = (((u32)&T0TIMI_Addr)+0x00007E0)<<16; // Read the interrupt vectors
                                                 // table address
  for(bCounter=0; bCounter<32; bCounter++) // Initialize SIRn registers
  {
    EIC->SIR[bCounter] = dOffset|0xF0000000;
    dOffset += 0x00000004 << 16;
  }
}

/*******************************************************************************
* Function Name  : EIC_CurrentPriorityLevelConfig
* Description    : Change the current priority level of the srved IRQ routine
* Input 1        : New priority
* Output         : None
* Return         : None
*******************************************************************************/
void EIC_CurrentPriorityLevelConfig(u8 NewPriorityLevel)
{
  u32 dVal;
  dVal = EIC->ICR;              // Save ICR content
  EIC->ICR &= ~0x0001;          // Disable IRQ interrupts.
  EIC->CIPR = NewPriorityLevel; // Change the current priority
  EIC->ICR = dVal;              // Restore ICR content
}

/******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****/
