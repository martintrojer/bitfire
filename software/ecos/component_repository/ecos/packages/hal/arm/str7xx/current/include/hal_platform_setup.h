#ifndef CYGONCE_HAL_PLATFORM_SETUP_H
#define CYGONCE_HAL_PLATFORM_SETUP_H

/*=============================================================================
//
//      hal_platform_setup.h
//
//      Platform specific support for HAL (assembly code)
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
// Author(s):   gthomas
// Contributors:gthomas
// Date:        2001-07-12
// Purpose:     AT91/EB40 platform specific support routines
// Description: 
// Usage:       #include <cyg/hal/hal_platform_setup.h>
//
//####DESCRIPTIONEND####
//
//===========================================================================*/

//#if defined(CYG_HAL_STARTUP_ROM) || defined(CYG_HAL_STARTUP_ROMRAM)
#if 1

.EQU EIC_Base_addr       ,    0xFFFFF800@ EIC base address
.EQU ICR_off_addr        ,    0x00      @ Interrupt Control register offset
.EQU CIPR_off_addr       ,    0x08      @ Current Interrupt Priority Register offset
.EQU IVR_off_addr        ,    0x18      @ Interrupt Vector Register offset
.EQU FIR_off_addr        ,    0x1C      @ Fast Interrupt Register offset
.EQU IER_off_addr        ,    0x20      @ Interrupt Enable Register offset
.EQU IPR_off_addr        ,    0x40      @ Interrupt Pending Bit Register offset
.EQU SIR0_off_addr       ,    0x60      @ Source Interrupt Register 0

.EQU EMI_Base_addr       ,    0x6C000000@ EMI base address
.EQU BCON0_off_addr      ,    0x00      @ Bank 0 configuration register offset
.EQU BCON1_off_addr      ,    0x04      @ Bank 1 configuration register offset
.EQU BCON2_off_addr      ,    0x08      @ Bank 2 configuration register offset
.EQU BCON3_off_addr      ,    0x0C      @ Bank 3 configuration register offset

.EQU EMI_ENABLE          ,    0x8000
.EQU EMI_SIZE_16         ,    0x0001

.EQU GPIO2_Base_addr     ,    0xE0005000@ GPIO2 base address
.EQU PC0_off_addr        ,    0x00      @ Port Configuration Register 0 offset
.EQU PC1_off_addr        ,    0x04      @ Port Configuration Register 1 offset
.EQU PC2_off_addr        ,    0x08      @ Port Configuration Register 2 offset
.EQU PD_off_addr         ,    0x0C      @ Port Data Register offset

.EQU CPM_Base_addr       ,    0xA0000040@ CPM Base Address
.EQU BOOTCR_off_addr     ,    0x10      @ CPM - Boot Configuration Register
.EQU FLASH_mask          ,    0x0000    @ to remap FLASH at 0x0
.EQU RAM_mask            ,    0x0002    @ to remap RAM at 0x0
.EQU EXTMEM_mask         ,    0x0003    @ to remap EXTMEM at 0x0

#;|----------------------------------------------------------------------------------|
#;| - APB Bridge  (System Peripheral)                                               |
#;|----------------------------------------------------------------------------------|

.EQU APB1_base_addr      ,    0xC0000000          @ APB Bridge1 Base Address
.EQU APB2_base_addr      ,    0xE0000000          @ APB Bridge2 Base Address
.EQU CKDIS_off_addr      ,    0x10                @ APB Bridge1 - Clock Disable  Register
.EQU SWRES_off_addr      ,    0x14                @ APB Bridge1 - Software Reset Register
.EQU CKDIS1_config_all   ,    0x27FB              @ To enable/disable clock of all APB1's peripherals
.EQU SWRES1_config_all   ,    0x27FB              @ To reset all APB1's peripherals
.EQU CKDIS2_config_all   ,    0x7FDD              @ To enable/disable clock of all APB2's peripherals
.EQU SWRES2_config_all   ,    0x7FDD              @ To reset all APB2's peripherals

#;*******************************************************************************
#;*******                         -- MACROS --                            *******
#;*******************************************************************************
#;*******************************************************************************
#;* Macro Name     : EMI_INIT
#;* Description    : This macro Initialize EMI bank 1: 16-bit 7 wait state
#;* Input          : None.
#;* Output         : None.
#;*******************************************************************************
	.macro EMI_INIT
        LDR     r0, =GPIO2_Base_addr      @ Configure P2.0 -> 3 in AF_PP mode
        LDR     r2, [r0, #PC0_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC0_off_addr]
        LDR     r2, [r0, #PC1_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC1_off_addr]
        LDR     r2, [r0, #PC2_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC2_off_addr]
        LDR     r0, =EMI_Base_addr
        LDR     r1, =0x04 | EMI_ENABLE | EMI_SIZE_16  @ 0x18
        STR     r1, [r0, #BCON1_off_addr] @ Enable bank 1 16-bit 7 wait state
        .endm

				
#;*******************************************************************************
#;* Macro Name     : EIC_INIT
#;* Description    : This macro Initialize the EIC as following :
#;                 - IRQ disabled
#;                 - FIQ disabled
#;                 - IVR contain the load PC opcode (0xE59FFXXX)
#;                 - Current priority level equal to 0
#;                 - All channels are disabled
#;                 - All channels priority equal to 0
#;                 - All SIR registers contain offset to the related IRQ
#;                   table entry
#;* Input          : None.
#;* Output         : None.
#;*******************************************************************************
	.macro EIC_INIT
        LDR     r3, =EIC_Base_addr
        LDR     r4, =0xE59F0000

        STR     r4, [r3, #IVR_off_addr]@ Write the LDR pc,[pc,#offset]
                                       @ instruction code in IVR[31:16]
        LDR     r2, =32                @ 32 Channel to initialize
        LDR     r0, =0                 @ Read the address of the IRQs
                                       @ address table
        LDR     r1, =0x00000FFF
        AND     r0,r0,r1
        LDR     r5, =SIR0_off_addr     @ Read SIR0 address
        SUB     r4,r0,#8               @ Subtract 8 for prefetch
        LDR     r1, =0xF7E8            @ Add the offset from IVR to 0x00000000
                                       @ address(IVR address + 7E8 = 0x00000000)
                                       @ 0xF7E8 used to complete the
                                       @ LDR pc,[pc,#offset] opcode (0xE59FFXXX)
        ADD     r1,r4,r1               @ Compute the jump offset from IVR to the
                                       @ IRQ table entry.
EIC_INI: MOV     r4, r1, LSL #16        @ Left shift the result
        STR     r4, [r3, r5]           @ Store the result in SIRx register
        ADD     r1, r1, #4             @ Next IRQ address
        ADD     r5, r5, #4             @ Next SIR
        SUBS    r2, r2, #1             @ Decrement the number of SIR registers
                                       @ to initialize
        BNE     EIC_INI                @ If more then continue
        .endm
#;*******************************************************************************
#;* Macro Name     : PERIPHERAL_INIT
#;* Description    : This macro reset all device peripherals.
#;* Input          : None.
#;* Output         : None.
#;*******************************************************************************
	.macro PERIPHERAL_INIT
        LDR     r1, =APB1_base_addr      @ r0= APB1 base address
        LDR     r2, =APB2_base_addr      @ r0= APB2 base address
        LDR     r0, =CKDIS1_config_all
        STRH    r0, [r1, #CKDIS_off_addr]@ Clock Disabling for all APB1 peripherals
        LDR     r0, =CKDIS2_config_all
        STRH    r0, [r2, #CKDIS_off_addr]@ Clock Disabling for all APB2 peripherals
        LDR     r0, =SWRES1_config_all
        STRH    r0, [r1, #SWRES_off_addr]@ Keep all APB1 peripherals under reset 
        LDR     r0, =SWRES2_config_all
        STRH    r0, [r2, #SWRES_off_addr]@ Keep all APB2 peripherals under reset 
        MOV     r7, #10                  @ Wait that the selected macrocells exit from reset
loop1:	SUBS    r7, r7, #1
        BNE     loop1
        MOV     r0, #0
        STRH    r0, [r1, #SWRES_off_addr]@ Enable all all APB1 peripherals
        STRH    r0, [r2, #SWRES_off_addr]@ Enable all all APB2 peripherals
        STRH    r0, [r1, #CKDIS_off_addr]@ Clock Enabling for all APB1 peripherals
        STRH    r0, [r2, #CKDIS_off_addr]@ Clock Enabling for all APB2 peripherals
        MOV     r7, #10                  @ Wait that the selected macrocells exit from reset
loop2:	SUBS    r7, r7, #1
        BNE     loop2
        .endm

#; define remapping
#; If you need to remap memory before entring the main program
#; uncomment next ligne
#;            GBLL  remapping
#.equ remapping, 1
			
#; Then define which memory to remap to address 0x00000000
#;  Uncomment next line if you want to remap RAM
#;            GBLL  remap_ram
#.equ remap_ram,1
	
#;  Uncomment next line if you want to remap FLASH
#;            GBLL  remap_flash
#.equ remap_flash,1
	
#;  Uncomment next line if you want to remap the external memory
#;            GBLL  remap_ext
#.equ remap_ext, 1

#if defined(CYG_HAL_STARTUP_RAM)
.equ remapping, 1
.equ remap_ram,1
#endif

    .macro  _setup
#; Uncomment next ligne if you need to reset all device pripherals
#        PERIPHERAL_INIT           @ Reset all device peripherals

#; Uncomment next ligne if you need to enable the EMI Bank 1

#if defined(CYG_HAL_STARTUP_ROM) || defined(CYG_HAL_STARTUP_ROMRAM)
        EMI_INIT                  @ Initialize EIM Bank 1
#endif

#;Uncomment next ligne if you need to initialize the EIC
        EIC_INIT                  @ Initialize EIC


#;******************************************************************************

	#;REMAPPING
#;Description  : Remapping  memory whether RAM,FLASH or External memory
#;               at Address 0x0 after the application has started executing.
#;               Remapping is generally done to allow RAM  to replace FLASH
#;               or EXTMEM at 0x00000000.
#;               the remapping of RAM allow copying of vector table into RAM
#;               To enable the memory remapping uncomment: (see above)
#;               GBLL  remapping to enable memory remapping
#;                  AND
#;               GBLL  remap_ram to remap RAM
#;                  OR
#;               GBLL  remap_flash to remap FLASH
#;                  OR
#;               GBLL  remap_ext to remap External memory Bank0
#;******************************************************************************
.ifdef remapping
	.ifdef remap_flash
	        MOV     r0, #FLASH_mask
	.endif
	.ifdef remap_ram
	        MOV     r0, #RAM_mask
	.endif
	.ifdef remap_ext
		MOV     r0, #EXTMEM_mask
	.endif
        LDR     r1, =CPM_Base_addr
        LDRH    r2, [r1, #BOOTCR_off_addr]@ Read BOOTCR Register
        BIC     r2, r2, #0x03             @ Reset the two LSB bits of BOOTCR
        ORR     r2, r2, r0                @ change the two LSB bits of BOOTCR
        STRH    r2, [r1, #BOOTCR_off_addr]@ Write BOOTCR Register
.endif

        .endm
#define CYGSEM_HAL_ROM_RESET_USES_JUMP
#define PLATFORM_SETUP1     _setup
#else
#define PLATFORM_SETUP1
#endif

//-----------------------------------------------------------------------------
// end of hal_platform_setup.h
#endif // CYGONCE_HAL_PLATFORM_SETUP_H
