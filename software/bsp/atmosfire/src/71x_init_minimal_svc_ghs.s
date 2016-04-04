//; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR            .equ    0x10
Mode_FIQ            .equ    0x11
Mode_IRQ            .equ    0x12
Mode_SVC            .equ    0x13
Mode_ABT            .equ    0x17
Mode_UNDEF          .equ    0x1B
Mode_SYS            .equ    0x1F // available on ARM Arch 4 and later

I_Bit               .equ    0x80 // when I bit is set, IRQ is disabled
F_Bit               .equ    0x40 // when F bit is set, FIQ is disabled

//; --- System memory locations

RAM_Base            .equ    0x20000000
RAM_Limit           .equ    0x20010000
SRAM_Base           .equ    0x62000000
SRAM_Limit		 .equ    0x62100000
Stack_Base          .equ    RAM_Limit

USR_Stack_Length    .equ    4096
IRQ_Stack_Length    .equ    1024
SVC_Stack_Length    .equ    256
FIQ_Stack_Length    .equ    256
ABT_Stack_Length    .equ    256
UNDEF_Stack_Length  .equ    256

USR_Stack           .equ    Stack_Base                 // USR stack
IRQ_Stack           .equ    USR_Stack-USR_Stack_Length // followed by IRQ stack
SVC_Stack           .equ    IRQ_Stack-IRQ_Stack_Length // followed by SVC stack
FIQ_Stack           .equ    SVC_Stack-SVC_Stack_Length // followed by FIQ stack
ABT_Stack           .equ    FIQ_Stack-FIQ_Stack_Length // followed by ABT stack
UNDEF_Stack         .equ    ABT_Stack-ABT_Stack_Length // followed by UNDEF stack
    
EIC_Base_addr       .equ    0xFFFFF800// EIC base address
ICR_off_addr        .equ    0x00      // Interrupt Control register offset
CIPR_off_addr       .equ    0x08      // Current Interrupt Priority Register offset
IVR_off_addr        .equ    0x18      // Interrupt Vector Register offset
FIR_off_addr        .equ    0x1C      // Fast Interrupt Register offset
IER_off_addr        .equ    0x20      // Interrupt Enable Register offset
IPR_off_addr        .equ    0x40      // Interrupt Pending Bit Register offset
SIR0_off_addr       .equ    0x60      // Source Interrupt Register 0

EMI_Base_addr       .equ    0x6C000000// EMI base address
BCON0_off_addr      .equ    0x00      // Bank 0 configuration register offset
BCON1_off_addr      .equ    0x04      // Bank 1 configuration register offset
BCON2_off_addr      .equ    0x08      // Bank 2 configuration register offset
BCON3_off_addr      .equ    0x0C      // Bank 3 configuration register offset

EMI_ENABLE          .equ    0x8000
EMI_SIZE_16         .equ    0x0001

GPIO2_Base_addr     .equ    0xE0005000// GPIO2 base address
PC0_off_addr        .equ    0x00      // Port Configuration Register 0 offset
PC1_off_addr        .equ    0x04      // Port Configuration Register 1 offset
PC2_off_addr        .equ    0x08      // Port Configuration Register 2 offset
PD_off_addr         .equ    0x0C      // Port Data Register offset

CPM_Base_addr       .equ    0xA0000040// CPM Base Address
BOOTCR_off_addr     .equ    0x10      // CPM - Boot Configuration Register
FLASH_mask          .equ    0x0000    // to remap FLASH at 0x0
RAM_mask            .equ    0x0002    // to remap RAM at 0x0
EXTMEM_mask         .equ    0x0003    // to remap EXTMEM at 0x0

//;|----------------------------------------------------------------------------------|
//;| - APB Bridge  (System Peripheral)                                               |
//;|----------------------------------------------------------------------------------|

APB1_base_addr      .equ    0xC0000000          // APB Bridge1 Base Address
APB2_base_addr      .equ    0xE0000000          // APB Bridge2 Base Address
CKDIS_off_addr      .equ    0x10                // APB Bridge1 - Clock Disable  Register
SWRES_off_addr      .equ    0x14                // APB Bridge1 - Software Reset Register
CKDIS1_config_all   .equ    0x27FB              // To enable/disable clock of all APB1's peripherals
SWRES1_config_all   .equ    0x27FB              // To reset all APB1's peripherals
CKDIS2_config_all   .equ    0x7FDD              // To enable/disable clock of all APB2's peripherals
SWRES2_config_all   .equ    0x7FDD              // To reset all APB2's peripherals
	


.text
	.force_nothumb

	.global _startup
	.global _start
_start:
_startup:
	LDR     PC, Reset_Addr
	LDR     PC, Undefined_Addr
	LDR     PC, SWI_Addr
	LDR     PC, Prefetch_Addr
	LDR     PC, Abort_Addr
	NOP                             // Reserved vector
	LDR     PC, IRQ_Addr
	LDR     PC, FIQ_Addr

Reset_Addr:     .word   Reset_Handler
Undef_Addr:     .word   Undef_Handler
SWI_Addr:       .word   SWI_Handler
PAbt_Addr:      .word   PAbt_Handler
DAbt_Addr:      .word   DAbt_Handler
				.word   0                      /* Reserved Address */
IRQ_Addr:       .word   IRQ_Handler
FIQ_Addr:       .word   FIQ_Handler

Undef_Handler:  B       Undef_Handler
SWI_Handler:    B       SWI_Handler
PAbt_Handler:   B       PAbt_Handler
DAbt_Handler:   B       DAbt_Handler
IRQ_Handler:    B       IRQHandler	
FIQ_Handler:    B       FIQ_Handler

// Reset Handler

Reset_Handler: 
		
		Reset_Handler:	
		LDR     pc, =NextInst
NextInst:	
		NOP		// Wait for OSC stabilization
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		
		MSR     CPSR_c, #Mode_ABT | F_Bit | I_Bit
        LDR     SP, =ABT_Stack

        MSR     CPSR_c, #Mode_UNDEF | F_Bit | I_Bit
        LDR     SP,= UNDEF_Stack

        MSR     CPSR_c, #Mode_SYS | F_Bit | I_Bit
        LDR     SP, =RAM_Limit
				

//		PERIPHERAL_INIT
		
		LDR     r1, =APB1_base_addr      // r0= APB1 base address
        LDR     r2, =APB2_base_addr      // r0= APB2 base address
        LDR     r0, =CKDIS1_config_all
        STRH    r0, [r1, #CKDIS_off_addr]// Clock Disabling for all APB1 peripherals
        LDR     r0, =CKDIS2_config_all
        STRH    r0, [r2, #CKDIS_off_addr]// Clock Disabling for all APB2 peripherals
        LDR     r0, =SWRES1_config_all
        STRH    r0, [r1, #SWRES_off_addr]// Keep all APB1 peripherals under reset 
        LDR     r0, =SWRES2_config_all
        STRH    r0, [r2, #SWRES_off_addr]// Keep all APB2 peripherals under reset 
        MOV     r7, #10                  // Wait that the selected macrocells exit from reset
loop1:	SUBS    r7, r7, #1
        BNE     loop1
        MOV     r0, #0
        STRH    r0, [r1, #SWRES_off_addr]// Enable all all APB1 peripherals
        STRH    r0, [r2, #SWRES_off_addr]// Enable all all APB2 peripherals
        STRH    r0, [r1, #CKDIS_off_addr]// Clock Enabling for all APB1 peripherals
        STRH    r0, [r2, #CKDIS_off_addr]// Clock Enabling for all APB2 peripherals
        MOV     r7, #10                  // Wait that the selected macrocells exit from reset
loop2:	SUBS    r7, r7, #1
        BNE     loop2


	    MSR     CPSR_c, #Mode_FIQ | I_Bit // Change to FIQ mode
        LDR     SP, =FIQ_Stack            // Initialize FIQ stack pointer

       	MSR     CPSR_c, #Mode_IRQ | I_Bit // Change to IRQ mode
        LDR     SP, =IRQ_Stack            // Initialize IRQ stack pointer

# mtrojer//arrownordic.com        
	MSR     CPSR_c, #Mode_SVC
# run in svc mode
	
#        MSR     CPSR_c, #Mode_USR        // Change to User mode, Enable IRQ and FIQ
        LDR     SP, =USR_Stack            // Initialize USR stack pointer

# mtrojer//arrownordic.com	
	
# Relocate .data section (Copy from ROM to RAM)
                LDR     R1, =_etext
                LDR     R2, =_data
                LDR     R3, =_edata
LoopRel:        CMP     R2, R3
                LDRLO   R0, [R1], #4
                STRLO   R0, [R2], #4
                BLO     LoopRel

# Clear .bss section (Zero init)
                MOV     R0, #0
                LDR     R1, =__bss_start__
                LDR     R2, =__bss_end__
LoopZI:         CMP     R1, R2
                STRLO   R0, [R1], #4
                BLO     LoopZI

		B main
				
/* Change This */
.global  IRQHandler
IRQHandler:	
        SUB    lr,lr,#4       // Update the link register
        SaveContext r0,r12    // Save the workspace plus the current
                              // return address lr_ irq and spsr_irq.
        LDR    lr, =ReturnAddress // Read the return address.
        LDR    r0, =EIC_base_addr
        LDR    r1, =IVR_off_addr
        ADD    pc,r0,r1      // Branch to the IRQ handler.
ReturnAddress:	
                             // Clear pending bit in EIC (using the proper IPRx)
        LDR    r0, =EIC_base_addr
        LDR    r2, [r0, #CICR_off_addr] // Get the IRQ channel number.
        MOV    r3,#1
        MOV    r3,r3,LSL r2
        STR    r3,[r0, #IPR_off_addr]   // Clear the corresponding IPR bit.
        RestoreContext r0,r12  // Restore the context and return to the...
                               // ...program execution.