;******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
;* File Name          : 71x_init.s
;* Author             : MCD Application Team
;* Date First Issued  : 16/05/2003
;* Description        : This is the first code executed after RESET.
;*                      This code used to initialize system stacks
;*                      and critical peripherals before entering the C code.
;*******************************************************************************
;* History:
;*  16/05/2003 : creation
;******************************************************************************/

        PRESERVE8
        AREA    Init, CODE, READONLY


; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR           EQU     0x10
Mode_FIQ           EQU     0x11
Mode_IRQ           EQU     0x12
Mode_SVC           EQU     0x13
Mode_ABT           EQU     0x17
Mode_UNDEF         EQU     0x1B
Mode_SYS           EQU     0x1F ; available on ARM Arch 4 and later

I_Bit              EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit              EQU     0x40 ; when F bit is set, FIQ is disabled


; --- System memory locations

RAM_Base            EQU    0x20000000
RAM_Limit           EQU    0x20010000
;SRAM_Base           EQU    0x62000000

;Total stack Size: (3K - 256) bytes
SVC_Stack           EQU     RAM_Limit         ; 256 byte SVC stack at top of memory
IRQ_Stack           EQU     SVC_Stack-256    ; followed by IRQ stack 1024 bytes
USR_Stack           EQU     IRQ_Stack-1024    ; followed by USR stack
FIQ_Stack           EQU     USR_Stack-1024     ; followed by FIQ stack
ABT_Stack           EQU     FIQ_Stack-256     ; followed by ABT stack
UNDEF_Stack         EQU     ABT_Stack-256     ; followed by UNDEF stack

EIC_Base_addr       EQU    0xFFFFF800         ; EIC base address
ICR_off_addr        EQU    0x00               ; Interrupt Control register offset
CIPR_off_addr       EQU    0x08               ; Current Interrupt Priority Register offset
IVR_off_addr        EQU    0x18               ; Interrupt Vector Register offset
FIR_off_addr        EQU    0x1C               ; Fast Interrupt Register offset
IER_off_addr        EQU    0x20               ; Interrupt Enable Register offset
IPR_off_addr        EQU    0x40               ; Interrupt Pending Bit Register offset
SIR0_off_addr       EQU    0x60               ; Source Interrupt Register 0

EMI_Base_addr       EQU    0x6C000000         ; EMI base address
BCON0_off_addr      EQU    0x00               ; Bank 0 configuration register offset
BCON1_off_addr      EQU    0x04               ; Bank 1 configuration register offset
BCON2_off_addr      EQU    0x08               ; Bank 2 configuration register offset
BCON3_off_addr      EQU    0x0C               ; Bank 3 configuration register offset

GPIO2_Base_addr     EQU    0xE0005000         ; GPIO2 base address
PC0_off_addr        EQU    0x00               ; Port Configuration Register 0 offset
PC1_off_addr        EQU    0x04               ; Port Configuration Register 1 offset
PC2_off_addr        EQU    0x08               ; Port Configuration Register 2 offset
PD_off_addr         EQU    0x0C               ; Port Data Register offset

CPM_Base_addr       EQU    0xA0000040         ; CPM Base Address
BOOTCONF_off_addr   EQU    0x10               ; CPM - Boot Configuration Register
FLASH_mask          EQU    0x0000             ; to remap FLASH at 0x0
RAM_mask            EQU    0x0002             ; to remap RAM at 0x0
EXTMEM_mask         EQU    0x0003             ; to remap EXTMEM at 0x0


        ENTRY

        EXPORT  Reset_Handler
        IMPORT  T0TIMI_Addr
        ;IMPORT  vector_begin
        ;IMPORT  vector_end
        
        ;We use linker default symbols instead of a scat file
        IMPORT	|Image$$ZI$$Limit|
        IMPORT	|Image$$ZI$$Base|

Reset_Handler
;         LDR     pc, =NextInst

;NextInst

		NOP		; Wait for OSC stabilization
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP

        MSR     CPSR_c, #Mode_ABT:OR:F_Bit:OR:I_Bit
        LDR     SP, =ABT_Stack

        MSR     CPSR_c, #Mode_UNDEF:OR:F_Bit:OR:I_Bit
        LDR     SP, =UNDEF_Stack

        MSR     CPSR_c, #Mode_SVC:OR:F_Bit:OR:I_Bit
        LDR     SP, =RAM_Limit


;******************************************************************************
;REMAPPING
;Description  : Remapping  memory whether RAM,FLASH or External memory
;               at Address 0x0 after the application has started executing.
;               Remapping is generally done to allow RAM  to replace FLASH
;               or EXTMEM at 0x0.
;               the remapping of RAM allow copying of vector table into RAM
;******************************************************************************

  	;first we check if RAM is already mapped.Skip if so.
  		LDR		r1, =CPM_Base_addr
  		LDRH	r2, [r1, #BOOTCONF_off_addr];Read BOOTCONF Register
  		AND		r2, r2, #0x03
  		CMP		r2, #RAM_mask
  		BEQ		end_remap
  		
    ;copying whole image into RAM
        LDR     r0, =0           			;r0 = start address from which to copy
        LDR     r3, =|Image$$ZI$$Base|      ;r3 = number of bytes to copy
        LDR     r1, =RAM_Base               ;r1 = start address where to copy
copy_ram
        LDR     r2, [r0], #4                ;Read a word from the source
        STR     r2, [r1], #4                ;copy the word to destination
        SUBS    r3, r3, #4                  ;Decrement number of words to copy
        BNE     copy_ram
        

        MOV     r0, #RAM_mask
	;now do mapping
        LDR     r1, =CPM_Base_addr
        LDRH    r2, [r1, #BOOTCONF_off_addr];Read BOOTCONF Register
        BIC     r2, r2, #0x03               ;Reset the two LSB bits of BOOTCONF Register
        ORR     r2, r2, r0                  ;change the two LSB bits of BOOTCONF Register
        STRH    r2, [r1, #BOOTCONF_off_addr];Write BOOTCONF Register
end_remap
;now zero init
		LDR 	r1,	 =|Image$$ZI$$Base|
		LDR		r3,  =|Image$$ZI$$Limit|
		MOV 	r2, #0
zero_init		
		STR		r2, [r1], #4
		CMP		r1, r3
		BCC		zero_init

;******************************************************************************
;EIC initialization
;Description  : Initialize the EIC as following :
;              - IRQ disabled
;              - FIQ disabled
;              - IVR contain the load PC opcode (0xF59FF00)
;              - Current priority level equal to 0
;              - All channels are disabled
;              - All channels priority equal to 0
;              - All SIR registers contain offset to the related IRQ table entry
;******************************************************************************
EIC_INIT
        LDR     r3, =EIC_Base_addr
        LDR     r4, =0x00000000
        STR     r4, [r3, #ICR_off_addr]   ; Disable FIQ and IRQ
        STR     r4, [r3, #IER_off_addr]   ; Disable all channels interrupts
        LDR     r4, =0xFFFFFFFF
        STR     r4, [r3, #IPR_off_addr]   ; Clear all IRQ pending bits
        LDR     r4, =0x0C
        STR     r4, [r3, #FIR_off_addr]   ; Disable FIQ channels and clear FIQ pending bits
        LDR     r4, =0x00000000
        STR     r4, [r3, #CIPR_off_addr]  ; Reset the current priority register
        LDR     r4, =0xE59F0000
        STR     r4, [r3, #IVR_off_addr]   ; Write the LDR pc,pc,#offset instruction code in IVR[31:16]
        LDR     r2, =32                   ; 32 Channel to initialize
        LDR     r0, =T0TIMI_Addr          ; Read the address of the IRQs address table
        LDR     r1, =0x00000FFF
        AND     r0,r0,r1
        LDR     r5, =SIR0_off_addr        ; Read SIR0 address
        SUB     r4,r0,#8                  ; subtract 8 for prefetch
        LDR     r1, =0xF7E8               ; add the offset to the 0x00000000 address(IVR address + 7E8 = 0x00000000)
                                          ; 0xF7E8 used to complete the LDR pc,pc,#offset opcode
        ADD     r1,r4,r1                  ; compute the jump offset
EIC_INI MOV     r4, r1, LSL #16           ; Left shift the result
        STR     r4, [r3, r5]              ; Store the result in SIRx register
        ADD     r1, r1, #4                ; Next IRQ address
        ADD     r5, r5, #4                ; Next SIR
        SUBS    r2, r2, #1                ; Decrement the number of SIR registers to initialize
        BNE     EIC_INI                   ; If more then continue

;        MSR     CPSR_c, #Mode_USR        ; Change to User mode, Enable IRQ and FIQ
;        LDR     SP, =USR_Stack           ; Initialize USR stack pointer

       	MSR     CPSR_c, #Mode_FIQ        ; Change to FIQ mode
        LDR     SP, =FIQ_Stack           ; Initialize FIQ stack pointer

       	MSR     CPSR_c, #Mode_IRQ        ; Change to IRQ mode
        LDR     SP, =IRQ_Stack           ; Initialize IRQ stack pointer

        MSR     CPSR_c, #Mode_SYS        ; Change to System mode
         LDR     SP, =USR_Stack

        IMPORT  platformmain

; --- Now enter the C code
        B       platformmain   ; Note : use B not BL, because an application will
                         ; never return this way

        LTORG


        END
;******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****
