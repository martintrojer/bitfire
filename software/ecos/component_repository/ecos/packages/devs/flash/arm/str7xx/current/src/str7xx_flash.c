//==========================================================================
//
//      str7xx_flash.c
//
//      Flash programming for the str7xx devices which have the 
//      Embedded Flash Controller.
//
//==========================================================================
//####ECOSGPLCOPYRIGHTBEGIN####
// -------------------------------------------
// This file is part of eCos, the Embedded Configurable Operating System.
// Copyright (C) 1998, 1999, 2000, 2001, 2002 Red Hat, Inc.
// Copyright (C) 2006 eCosCentric Ltd
// Copyright (C) 2006 Andrew Lunn (andrew.lunn@ascom.ch)
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
// -------------------------------------------
//####ECOSGPLCOPYRIGHTEND####
//==========================================================================
//#####DESCRIPTIONBEGIN####
//
// Author(s):    mtrojer
// Contributors: 
// Date:         2007-04-24
// Purpose:      
// Description:  
//              
//####DESCRIPTIONEND####
//
//==========================================================================

#include <pkgconf/hal.h>
#include <pkgconf/devs_flash_str7xx.h>
#include <str7xx_flash.h>

#include <cyg/hal/hal_io.h>
#include <cyg/hal/hal_intr.h>
#include <cyg/infra/cyg_ass.h>

#define  _FLASH_PRIVATE_
#include <cyg/io/flash.h>

#include <string.h>

#define FLASH_TIMEOUT       100000

/*******************************************************************************
* Function Name  : FLASH_Init
* Description    : Initialise the Flash registers to their default values.
* Input          : None.
* Return         : None.
*******************************************************************************/
static void FLASH_Init(void)
{
  /* Reset Flash Control Registers */
  FLASHR->CR0 = 0x00000000;
  FLASHR->CR1 = 0x00000000;
  /* Reset Flash Data Registers */
  FLASHR->DR0 = 0xFFFFFFFF;
  FLASHR->DR1 = 0xFFFFFFFF;
  /* Reset Flash Error Register */
  FLASHR->ER  = 0x00000000;
}

/*******************************************************************************
* Function Name  : FLASH_WordWrite
* Description    : Writes a Word in the Flash.
* Input 1        : Address of the Destination.
* Input 2        : Word to program at Address.
* Return         : None.
*******************************************************************************/
static void FLASH_WordWrite(u32 XtargetAdd, u32 Xdata)
{
  /* Wait until another operation going on is completed */
  FLASH_WaitForLastTask();
  /* Set the Word Programming bit 'WPG' in the CR0 Reg */
  FLASHR->CR0 |= FLASH_WPG_Mask;
  /* Load the destination address in AR */
  FLASHR->AR   = XtargetAdd;
  /* Load DATA to be programmed in DR0 */
  FLASHR->DR0  = Xdata;
  /* Set the Write Mode Start bit 'WMS' in the CR0 Reg to Start Write Operation */
  FLASHR->CR0 |= FLASH_WMS_Mask;
  /* Wait until the write operation is completed */
  FLASH_WaitForLastTask();
}

#if 0
/*******************************************************************************
* Function Name  : FLASH_DWordWrite
* Description    : Writes Double Word to the Flash.
* Input 1        : Address of the Destination.
* Input 2        : Word 1 to program.
* Input 3        : Word 2 to program.
* Return         : None.
*******************************************************************************/
static void FLASH_DWordWrite(u32 XtargetAdd, u32 Xdata0, u32 Xdata1)
{
  /* Wait until another operation going on is completed */
  FLASH_WaitForLastTask();
  /* set the Double Word Programming bit 'DWPG' in the CR0 Reg */
  FLASHR->CR0 |= FLASH_DWPG_Mask;
  /* Load the destination address in AR */
  FLASHR->AR   = XtargetAdd;
  /* Load DATA0 in DR0 Reg */
  FLASHR->DR0  = Xdata0;
  /* Load DATA1 in DR1 Reg */
  FLASHR->DR1  = Xdata1;
  /* Set the Write Mode Start bit 'WMS' in the CR0 Reg to Start Write Operation */
  FLASHR->CR0 |= FLASH_WMS_Mask;
  /* Wait until the write operation is completed */
  FLASH_WaitForLastTask();
}
#endif

/*******************************************************************************
* Function Name  : FLASH_SectorErase
* Description    : Erases a Flash sector.
* Input 1        : Sectors to be Erased.
* Return         : None.
*******************************************************************************/
static void FLASH_SectorErase(u32 Xsectors)
{
  /* Wait until another operation going on is completed */  
  FLASH_WaitForLastTask();
  /* Set the Sector Erase flag 'SER' in the CRO reg */
  FLASHR->CR0 |= FLASH_SER_Mask;
  /* Select in the CR1 register the Sectors to be erased  */
  FLASHR->CR1 |= Xsectors;
  /* Set the Write Mode Start bit 'WMS' in the CR0 Reg to Start Erase Operation */
  FLASHR->CR0 |= FLASH_WMS_Mask;
  /* Wait until the erase operation is completed */
  FLASH_WaitForLastTask();
}

#if 0
/*******************************************************************************
* Function Name  : FLASH_Suspend
* Description    : Suspends the current program or erase operation.
* Input 1        : None.
* Return         : Flash CR0 register content.
*******************************************************************************/
static u32 FLASH_Suspend(void)
{
  u32 temp;
  /* Save the CR0 Content */
  temp = FLASHR->CR0;
  /* Set The suspend Bit 'SUSP' in the CR0 register */
  FLASHR->CR0 |= FLASH_SUSP_Mask;
  /* Wait until the flash controller acknowledges the suspend of the current
  operation */
  FLASH_WaitForLastTask();
  return temp;
}
#endif

#if 0
/*******************************************************************************
* Function Name  : FLASH_Resume
* Description    : Resume a Suspended program or erase operation.
* Input 1        : The operation to be resumed.
* Return         : None.
*******************************************************************************/
static void FLASH_Resume(u32 FLASH_LastOperation)
{
  /* Clear The suspend Bit 'SUSP' in the FRC0 register */
  FLASHR->CR0 &= ~FLASH_SUSP_Mask;
  /* Resume last operation */
  FLASHR->CR0 = FLASH_LastOperation & 0x7FFFFFFF;
  /* Set write mode bit */
  FLASHR->CR0  |= FLASH_WMS_Mask;
}
#endif

#if 0
/*******************************************************************************
* Function Name  : FLASH_WordRead
* Description    : Read a single word from the flash
* Input 1        : Source Address
* Return         : Word
*******************************************************************************/
static u32 FLASH_WordRead(u32 FLASH_SourceAdd)
{
  FLASH_WaitForLastTask();
  /* Reads the data from the specified Address */
  return *(u32 *)(FLASH_SourceAdd + 0x40000000);
}
#endif

/*******************************************************************************
* Function Name  : FLASH_FlagStatus
* Description    : Returns the NewState of Flash flags
* Input          : Xflag(FLASH_BSYA0,  FLASH_BSYA1, FLASH_LOCK, FLASH_INTP
*                  FLASH_B0S, FLASH_B1S, FLASH_ERR, FLASH_ERER, FLASH_PGER, 
*                  FLASH_10ER, FLASH_SEQER, FLASH_RESER, FLASH_WPF)  
* Return         : FlagStatus (SET or RESET).
*******************************************************************************/
static FlagStatus FLASH_FlagStatus(flashflags Xflag)
{
  FlagStatus TmpResult = RESET;
  u8 TmpReg;
  u8 TmpPos;
  /* Get the Register Index */
  TmpReg = (Xflag & FLASH_Reg_Mask) >> 5;
  /* Get the Flag Index */
  TmpPos = (Xflag & FLASH_Flag_Mask);

  switch(TmpReg)
  {
    case 0 : /* CR0 */
    {
      /* Returns the status of the CR0[TmpPos] flag */
      TmpResult = (FLASHR->CR0 & (1<<TmpPos))==0 ? RESET : SET;
      break;
    }
    case 1 : /* CR1 */
    {
      /* Returns the status of the CR1[TmpPos] flag */
      TmpResult = (FLASHR->CR1 & (1<<TmpPos))==0 ? RESET : SET;
      break;
    }
    case 5 : /* ER */
    {
      /* Returns the status of the ER[TmpPos] flag */
      TmpResult = (FLASHR->ER  & (1<<TmpPos))==0 ? RESET : SET;
      break;
    }
  }
  return(TmpResult);
}

#if 0
/*******************************************************************************
* Function Name  : FLASH_FlagClear.
* Description    : Clears a flash flag.
* Input          : Xflag(FLASH_BSYA0,  FLASH_BSYA1, FLASH_LOCK, FLASH_INTP
*                  FLASH_B0S, FLASH_B1S, FLASH_ERR, FLASH_ERER, FLASH_PGER, 
*                  FLASH_10ER, FLASH_SEQER, FLASH_RESER, FLASH_WPF).  
* Return         : None.
*******************************************************************************/
static void FLASH_FlagClear(flashflags Xflag)
{
  u8 TmpReg, TmpPos;
  TmpReg = (Xflag & FLASH_Reg_Mask) >> 5;
  TmpPos = (Xflag & FLASH_Flag_Mask);

  switch(TmpReg)
  {
    case 0 : /* CR0 */
    {
      /* Clears the status of the CR0[TmpPos] flag */
      FLASHR->CR0 &= ~(1<<TmpPos);
      break;
    }
    case 1 : /* CR1 */
    {
      /* Clears the status of the CR1[TmpPos] flag */
      FLASHR->CR1 &= ~(1<<TmpPos);
      break;
    }
    case 5 : /* ER */
    {
      /* Clears the status of the ER[TmpPos] flag */
      FLASHR->ER &= ~(1<<TmpPos);
      break;
    }
  }
}
#endif

/*******************************************************************************
* Function Name  : FLASH_WritePrConfig
* Description    : Enable Write protection or Disable temporarily Write 
*                  protection of a flash sector.
* Input 1        : Flash Sector.
* Input 2        : Enable or disable Flash sector Write Protection.
* Return         : None.
*******************************************************************************/
static void FLASH_WritePrConfig(u32 Xsectors, FunctionalState NewState)
{
  /* Wait until another operation going on is completed */  
  FLASH_WaitForLastTask();  
  /* Enable Xsectors write protection */
  if (NewState == ENABLE)
  {
    /* Set the Set protection Bit */
    FLASHR->CR0 |= FLASH_SPR_Mask;
    /* Set the Register Address */
    FLASHR->AR  = 0x4010DFB0;
    /* Data to be programmed to the Protection Register */
    FLASHR->DR0  = ~Xsectors;
    /* Set the WMS bit to Start the Sequence */
    FLASHR->CR0 |= FLASH_WMS_Mask;
  }
  /* Unprotect temporarily Flash sector */ 
  else
  {
    /* Set the Set protection Bit */
    FLASHR->CR0 |= FLASH_SPR_Mask;
    /* Set the Register Address */
    FLASHR->AR  = 0x4010DFB0;
    /* Data to be programmed to the Protection Register */
    FLASHR->DR0  = FLASHPR->NVWPAR|Xsectors;
    /* Set the WMS bit to Start the Sequence */
    FLASHR->CR0 |= FLASH_WMS_Mask;
  }
}

#if 0
/*******************************************************************************
* Function Name  : FLASH_PermanentDebugPrConfig
* Description    : Configures the Debug Protection Bits
* Input 1        : NewState: ENABLE or DISABLE.
* Return         : None.
*******************************************************************************/
static void FLASH_PermanentDebugPrConfig(FunctionalState NewState)
{
  u16 ProtectionLevel = FLASH_ProtectionLevel();

  /* Enable permanent Debug protection */
  if (NewState == ENABLE)
  {
    /* If this is the first time that the debug protection is enabled:Reset 
    the DBGP bit in the NVAPR0 register */
    if(!ProtectionLevel) 
    {
      /* Set the Set protection Bit */
      FLASHR->CR0 |= FLASH_SPR_Mask;
      /* Set the Register Address */
      FLASHR->AR  = 0x4010DFB8;
      /* Data to be programmed to the Protection Register NVAPR0 */
      FLASHR->DR0 = ~2;
      /* Set the WMS bit to start the sequence */
      FLASHR->CR0 |= FLASH_WMS_Mask;
    }
    /* If this is not the first time that the debug protection is enabled: 
    use of NVAPR1 register */
    else 
    {
      /* Set the Set protection Bit */
      FLASHR->CR0 |= FLASH_SPR_Mask;
      /* Set the Register Address */
      FLASHR->AR  = 0x4010DFBC;
      /* Data to be programmed to the Protection Register NVAPR1(PEN bits) */
      FLASHR->DR0 =~(1<<(15+ProtectionLevel));
      /* Set the WMS bit to Start the Sequence */
      FLASHR->CR0 |= FLASH_WMS_Mask;
    }

  }
  /* Permanent debug unprotection: use of NVAPR1 register */ 
  else if(ProtectionLevel) 
  {
    /* Set the Set protection Bit */
    FLASHR->CR0 |= FLASH_SPR_Mask;
    /* Set the Register Address */
    FLASHR->AR  = 0x4010DFBC;
    /* Data to be programmed to the Protection Register NVAPR1(PDS bits)*/
    FLASHR->DR0 =~(1<<(ProtectionLevel-1));
    /* Set the WMS bit to Start the Sequence */
    FLASHR->CR0 |= FLASH_WMS_Mask;
  }

}
#endif

#if 0
/*******************************************************************************
* Function Name  : FLASH_ProtectionLevel
* Description    : If the flash is Debug protected, up to 16 unprotection/
*                  protection cycles are possible using the NVAPR1 register.
*                  This routine returns the number of times of Debug unprotection. 
* Input          : None.
* Return         : Number of times of Debug Protection/Unprotection(0..15)
*                  Example: if 5 is returned, this means that the flash was debug 
*                  unprotected 5 times(using PDS bits)and debug protected 5 times 
*                  (1 time:using DBGP bit & 4 times: using PEN bits.   
*******************************************************************************/
static u16 FLASH_ProtectionLevel(void)
{
  u16 TmpBitIndex =0;
  u16 ProtectionRegs;

  /* If DBGP = 1(Flash is Debug protected */
  if(FLASHPR->NVAPR0&2)
    TmpBitIndex=0;

  /*If DBGP = 0 (Flash is not Debug protected */
  else
    TmpBitIndex=1;
  ProtectionRegs=~(FLASHPR->NVAPR1>>16);

  while (((ProtectionRegs) != 0) && (TmpBitIndex < 16))
  {
    ProtectionRegs  = ProtectionRegs >>  1 ;
    TmpBitIndex++;
  }
  return TmpBitIndex;
}
#endif
/*******************************************************************************
* Function Name  : FLASH_WaitForLastTask
* Description    : Waits for the end of last task on a Flash Bank.
* Input 1        : None.
* Return         : None.
*******************************************************************************/
static void FLASH_WaitForLastTask(void)
{
  while (FLASHR->CR0&0x16);
}

//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

int flash_query(void *data) 
{
//  *data = (void)0;
  return FLASH_ERR_OK;
}

int flash_hwr_init(void)
{
  FLASH_Init () ;
  
  flash_info.block_size = 0x10000;
  flash_info.blocks = 4;
  flash_info.buffer_size = 0;
  flash_info.start = (void *) 0x40000000;
  flash_info.end =   (void *) 0x40040000; // Ignore Bank1

  return FLASH_ERR_OK;
}

int flash_erase_block (volatile unsigned long block) 
{
  cyg_uint32 fblock;
  
  switch(block)
  {
    case 0x40000000:
      FLASH_WritePrConfig (FLASH_B0F0,DISABLE) ;
      FLASH_WritePrConfig (FLASH_B0F1,DISABLE) ;
      FLASH_WritePrConfig (FLASH_B0F2,DISABLE) ;
      FLASH_WritePrConfig (FLASH_B0F3,DISABLE) ;
      FLASH_WritePrConfig (FLASH_B0F4,DISABLE) ;
      FLASH_SectorErase (FLASH_B0F0) ;
      FLASH_SectorErase (FLASH_B0F1) ;
      FLASH_SectorErase (FLASH_B0F2) ;
      FLASH_SectorErase (FLASH_B0F3) ;
      FLASH_SectorErase (FLASH_B0F4) ;
      break;
    case 0x40010000:
      FLASH_WritePrConfig (FLASH_B0F5,DISABLE) ;
      FLASH_SectorErase (FLASH_B0F5) ;
      break;
    case 0x40020000:
      FLASH_WritePrConfig (FLASH_B0F6,DISABLE) ;
      FLASH_SectorErase (FLASH_B0F6) ;
      break;
    case 0x40030000:
      FLASH_WritePrConfig (FLASH_B0F7,DISABLE) ;
      FLASH_SectorErase (FLASH_B0F7) ;
      break;
  }

  return FLASH_ERR_OK;
}

int flash_program_buf (volatile unsigned long addr, unsigned long *data, int len)
{  
  CYG_ASSERT(len % 4 == 0, "Only word writes allowed by current code");
  CYG_ASSERT(addr % 4 == 0, "Address must be word aligned for current code");

  while (len > 0) {
    FLASH_WordWrite (addr,*data) ;
    addr+=sizeof(unsigned long);
    data+=1;
    len = len - sizeof(unsigned long);
  }
  return FLASH_ERR_OK;
}

// Map a hardware status to a package error. NOP since the errors are
// already mapped.
int flash_hwr_map_error(int err){

  return err;
}

// See if a range of FLASH addresses overlaps currently running code
bool flash_code_overlaps(void *start, void *end){

    extern char _stext[], _etext[];

    return ((((unsigned long)&_stext >= (unsigned long)start) &&
             ((unsigned long)&_stext < (unsigned long)end)) ||
            (((unsigned long)&_etext >= (unsigned long)start) &&
             ((unsigned long)&_etext < (unsigned long)end)));
}
