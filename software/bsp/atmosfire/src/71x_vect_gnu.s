# Converted to GNU Assembler by Martin Trojer 2007.
# mtrojer@arrownordic.com
#;******************** (C) COPYRIGHT 2003 STMicroelectronics ********************
#;* File Name          : 71x_vect.s
#;* Author             : MCD Application Team
#;* Date First Issued  : 16/05/2003
#;* Description        : This file used to initialize the exception and IRQ
#;*                      vectors, and to enter/return to/from exceptions handlers.
#;*******************************************************************************
#;* History:
#;*  13/01/2006 : V3.1
#;*  24/05/2005 : V3.0
#;*  30/11/2004 : V2.0
#;*  14/07/2004 : V1.3
#;*  01/01/2004 : V1.2
#;*******************************************************************************
#; THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH
#; CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
#; AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
#; OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
#; OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
#; CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
#;*******************************************************************************/

	.section .data
	
.EQU EIC_base_addr         ,    0xFFFFF800@ EIC base address.
.EQU CICR_off_addr         ,    0x04      @ Current Interrupt Channel Register.
.EQU IVR_off_addr          ,    0x18      @ Interrupt Vector Register.
.EQU IPR_off_addr          ,    0x40      @ Interrupt Pending Register.

#;*******************************************************************************
#;              Import  the Reset_Handler address from 71x_init.s
#;*******************************************************************************
        .extern  Reset_Handler

#;*******************************************************************************
#;                      Import exception handlers
#;*******************************************************************************
        .extern  Undefined_Handler
        .extern  SWI_Handler
        .extern  Prefetch_Handler
        .extern  Abort_Handler
        .extern  FIQ_Handler

#;*******************************************************************************
#;                   Import IRQ handlers from 71x_it.c
#;*******************************************************************************
        .extern  T0TIMI_IRQHandler
        .extern  FLASH_IRQHandler
        .extern  RCCU_IRQHandler
        .extern  RTC_IRQHandler
        .extern  WDG_IRQHandler
        .extern  XTI_IRQHandler
        .extern  USBHP_IRQHandler
        .extern  I2C0ITERR_IRQHandler
        .extern  I2C1ITERR_IRQHandler
        .extern  UART0_IRQHandler
        .extern  UART1_IRQHandler
        .extern  UART2_IRQHandler
        .extern  UART3_IRQHandler
        .extern  BSPI0_IRQHandler
        .extern  BSPI1_IRQHandler
        .extern  I2C0_IRQHandler
        .extern  I2C1_IRQHandler
        .extern  CAN_IRQHandler
        .extern  ADC12_IRQHandler
        .extern  T1TIMI_IRQHandler
        .extern  T2TIMI_IRQHandler
        .extern  T3TIMI_IRQHandler
        .extern  HDLC_IRQHandler
        .extern  USBLP_IRQHandler
        .extern  T0TOI_IRQHandler
        .extern  T0OC1_IRQHandler
        .extern  T0OC2_IRQHandler

#;*******************************************************************************
#;            Export Peripherals IRQ handlers table address
#;*******************************************************************************

#;*******************************************************************************
#;
#;*******************************************************************************
        .global  vector_begin

	.section .text
	.arm
	.global _start

_start:	
vector_begin:	

#;*******************************************************************************
#;                        Exception vectors
#;*******************************************************************************
        LDR     PC, Reset_Addr
        LDR     PC, Undefined_Addr
        LDR     PC, SWI_Addr
        LDR     PC, Prefetch_Addr
        LDR     PC, Abort_Addr
        NOP                             @ Reserved vector
        LDR     PC, IRQ_Addr
        LDR     PC, FIQ_Addr

#;*******************************************************************************
#;               Exception handlers address table
#;*******************************************************************************
Reset_Addr:		.word     Reset_Handler
Undefined_Addr:		.word     UndefinedHandler
SWI_Addr:		.word     SWIHandler
Prefetch_Addr:		.word     PrefetchAbortHandler
Abort_Addr:		.word     DataAbortHandler
			.word     0               @ Reserved vector
IRQ_Addr:		.word     IRQHandler
FIQ_Addr:		.word     FIQHandler

#;*******************************************************************************
#;              Peripherals IRQ handlers address table
#;*******************************************************************************
	.global  T0TIMI_Addr
T0TIMI_Addr:		.word  T0TIMIIRQHandler
FLASH_Addr:		.word  FLASHIRQHandler
RCCU_Addr:		.word  RCCUIRQHandler
RTC_Addr:		.word  RTCIRQHandler
WDG_Addr:	        .word  WDGIRQHandler
XTI_Addr:	        .word  XTIIRQHandler
USBHP_Addr:	      .word  USBHPIRQHandler
I2C0ITERR_Addr:	  .word  I2C0ITERRIRQHandler
I2C1ITERR_ADDR:	  .word  I2C1ITERRIRQHandler
UART0_Addr:	      .word  UART0IRQHandler
UART1_Addr:	      .word  UART1IRQHandler
UART2_ADDR:	      .word  UART2IRQHandler
UART3_ADDR:	      .word  UART3IRQHandler
BSPI0_ADDR:	      .word  BSPI0IRQHandler
BSPI1_Addr:	      .word  BSPI1IRQHandler
I2C0_Addr:	       .word  I2C0IRQHandler
I2C1_Addr:	       .word  I2C1IRQHandler
CAN_Addr:	        .word  CANIRQHandler
ADC12_Addr:	      .word  ADC12IRQHandler
T1TIMI_Addr:	     .word  T1TIMIIRQHandler
T2TIMI_Addr:	     .word  T2TIMIIRQHandler
T3TIMI_Addr:	     .word  T3TIMIIRQHandler
                .word  0                  @ reserved
                .word  0                  @ reserved
                .word  0                  @ reserved
HDLC_Addr:	       .word  HDLCIRQHandler
USBLP_Addr:	      .word  USBLPIRQHandler
                .word  0                  @ reserved
                .word  0                  @ reserved
T0TOI_Addr:	      .word  T0TOIIRQHandler
T0OC1_Addr:	      .word  T0OC1IRQHandler
T0OC2_Addr:	      .word  T0OC2IRQHandler

        .global  vector_end
vector_end:	

#;*******************************************************************************
#;                         Exception Handlers
#;*******************************************************************************

#;*******************************************************************************
#;* Macro Name     : SaveContext
#;* Description    : This macro used to save the context before entering
#;                   an exception handler.
#;* Input          : The range of registers to store.
#;* Output         : none
#;*******************************************************************************
	.macro SaveContext reg1, reg2
        STMFD  sp!,{\reg1-\reg2,lr} @ Save The workspace plus the current return
                              @ address lr_ mode into the stack.
        MRS    r1,spsr        @ Save the spsr_mode into r1.
        STMFD  sp!,{r1}       @ Save spsr.
	.endm

#;*******************************************************************************
#;* Macro Name     : RestoreContext
#;* Description    : This macro used to restore the context to return from
#;                   an exception handler and continue the program execution.
#;* Input          : The range of registers to restore.
#;* Output         : none
#;*******************************************************************************
	.macro RestoreContext reg1 ,reg2
        LDMFD   sp!,{r1}        @ Restore the saved spsr_mode into r1.
        MSR     spsr_cxsf,r1    @ Restore spsr_mode.
        LDMFD   sp!,{\reg1-\reg2,pc}^ @ Return to the instruction following...
                                @ ...the exception interrupt.
	.endm

#;*******************************************************************************
#;* Function Name  : UndefinedHandler
#;* Description    : This function called when undefined instruction
#;                   exception is entered.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
UndefinedHandler:	
        SaveContext r0,r12    @ Save the workspace plus the current
                              @ return address lr_ und and spsr_und.
        BL      Undefined_Handler @ Branch to Undefined_Handler.
        RestoreContext r0,r12 @ Return to the instruction following...
                              @ ...the undefined instruction.

#;*******************************************************************************
#;* Function Name  : SWIHandler
#;* Description    : This function called when SWI instruction executed.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
SWIHandler:	
        SaveContext r0,r12    @ Save the workspace plus the current
                              @ return address lr_ svc and spsr_svc.
        BL       SWI_Handler  @ Branch to SWI_Handler.
        RestoreContext r0,r12 @ Return to the instruction following...
                              @ ...the SWI instruction.

#;*******************************************************************************
#;* Function Name  : IRQHandler
#;* Description    : This function called when IRQ exception is entered.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
        .global  IRQHandler
IRQHandler:	
        SUB    lr,lr,#4       @ Update the link register
        SaveContext r0,r12    @ Save the workspace plus the current
                              @ return address lr_ irq and spsr_irq.
        LDR    lr, =ReturnAddress @ Read the return address.
        LDR    r0, =EIC_base_addr
        LDR    r1, =IVR_off_addr
        ADD    pc,r0,r1      @ Branch to the IRQ handler.
ReturnAddress:	
                             @ Clear pending bit in EIC (using the proper IPRx)
        LDR    r0, =EIC_base_addr
        LDR    r2, [r0, #CICR_off_addr] @ Get the IRQ channel number.
        MOV    r3,#1
        MOV    r3,r3,LSL r2
        STR    r3,[r0, #IPR_off_addr]   @ Clear the corresponding IPR bit.
        RestoreContext r0,r12  @ Restore the context and return to the...
                               @ ...program execution.

#;*******************************************************************************
#;* Function Name  : PrefetchAbortHandler
#;* Description    : This function called when Prefetch Abort
#;                   exception is entered.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
PrefetchAbortHandler:	
        SUB    lr,lr,#4       @ Update the link register.
        SaveContext r0,r12    @ Save the workspace plus the current
                              @ return address lr_abt and spsr_abt.
        BL     Prefetch_Handler @ Branch to Prefetch_Handler.
        RestoreContext r0,r12 @ Return to the instruction following that...
                              @ ...has generated the prefetch abort exception.

#;*******************************************************************************
#;* Function Name  : DataAbortHandler
#;* Description    : This function is called when Data Abort
#;                   exception is entered.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
DataAbortHandler:	
        SUB    lr,lr,#8       @ Update the link register.
        SaveContext r0,r12    @ Save the workspace plus the current
                              @ return address lr_ abt and spsr_abt.
        BL     Abort_Handler  @ Branch to Abort_Handler.
        RestoreContext r0,r12 @ Return to the instruction following that...
                              @ ...has generated the data abort exception.

#;*******************************************************************************
#;* Function Name  : FIQHandler
#;* Description    : This function is called when FIQ
#;                   exception is entered.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
FIQHandler:	
        SUB    lr,lr,#4       @ Update the link register.
        SaveContext r0,r7     @ Save the workspace plus the current
                              @ return address lr_ fiq and spsr_fiq.
        BL      FIQ_Handler   @ Branch to FIQ_Handler.
        RestoreContext r0,r7  @ Restore the context and return to the...
                              @ ...program execution.

#;*******************************************************************************
#;* Macro Name     : IRQ_to_SYS
#;* Description    : This macro used to switch form IRQ mode to SYS mode
#;* Input          : none.
#;* Output         : none
#;*******************************************************************************
       .macro IRQ_to_SYS
        MSR    cpsr_c,#0x1F   @ Switch to SYS mode
        STMFD  sp!,{lr}       @ Save the link register.
       .endm
	
#;*******************************************************************************
#;* Macro Name     : SYS_to_IRQ
#;* Description    : This macro used to switch from SYS mode to IRQ mode
#;                   then to return to IRQHnadler routine.
#;* Input          : none.
#;* Output         : none.
#;*******************************************************************************
       .macro SYS_to_IRQ
        LDMFD  sp!,{lr}      @ Restore the link register.
        MSR    cpsr_c,#0xD2  @ Switch to IRQ mode.
        MOV    pc,lr         @ Return to IRQHandler routine to clear the
                             @ pending bit.
       .endm
	
#;*******************************************************************************
#;* Function Name  : T0TIMIIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T0TIMI_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T0TIMI_IRQHandler function termination.
#;* Input          : none.
#;* Output         : none.
#;*******************************************************************************
T0TIMIIRQHandler:	
        IRQ_to_SYS
        BL     T0TIMI_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : FLASHIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the FLASH_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   FLASH_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
FLASHIRQHandler:	
        IRQ_to_SYS
        BL     FLASH_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : RCCUIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the RCCU_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   RCCU_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
RCCUIRQHandler:	
        IRQ_to_SYS
        BL     RCCU_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : RTCIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the RTC_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   RTC_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
RTCIRQHandler:	
        IRQ_to_SYS
        BL     RTC_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : WDGIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the WDG_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   WDG_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
WDGIRQHandler:	
        IRQ_to_SYS
        BL     WDG_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : XTIIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the XTI_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   XTI_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
XTIIRQHandler:	
        IRQ_to_SYS
        BL     XTI_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : USBHPIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the USBHP_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   USBHP_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
USBHPIRQHandler:	
        IRQ_to_SYS
        BL     USBHP_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : I2C0ITERRIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the I2C0ITERR_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   I2C0ITERR_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
I2C0ITERRIRQHandler:	
        IRQ_to_SYS
        BL     I2C0ITERR_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : I2C1ITERRIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the I2C1ITERR_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   I2C1ITERR_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
I2C1ITERRIRQHandler:	
        IRQ_to_SYS
        BL     I2C1ITERR_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : UART0IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the UART0_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   UART0_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
UART0IRQHandler:	
        IRQ_to_SYS
        BL     UART0_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : UART1IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the UART1_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   UART1_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
UART1IRQHandler:	
        IRQ_to_SYS
        BL     UART1_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : UART2IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the UART2_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   UART2_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
UART2IRQHandler:	
        IRQ_to_SYS
        BL     UART2_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : UART3IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the UART3_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   UART3_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
UART3IRQHandler:	
        IRQ_to_SYS
        BL     UART3_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : BSPI0IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the BSPI0_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   BSPI0_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
BSPI0IRQHandler:	
        IRQ_to_SYS
        BL     BSPI0_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : BSPI1IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the BSPI1_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   BSPI1_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
BSPI1IRQHandler:	
        IRQ_to_SYS
        BL     BSPI1_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : I2C0IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the I2C0_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   I2C0_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
I2C0IRQHandler:	
        IRQ_to_SYS
        BL     I2C0_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : I2C1IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the I2C1_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   I2C1_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
I2C1IRQHandler:	
        IRQ_to_SYS
        BL     I2C1_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : CANIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the CAN_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   CAN_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
CANIRQHandler:	
        IRQ_to_SYS
        BL     CAN_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : ADC12IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the ADC12_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   ADC12_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
ADC12IRQHandler:	
        IRQ_to_SYS
        BL     ADC12_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : T1TIMIIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T1TIMI_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T1TIMI_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
T1TIMIIRQHandler:	
        IRQ_to_SYS
        BL     T1TIMI_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : T2TIMIIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T2TIMI_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T2TIMI_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
T2TIMIIRQHandler:	
        IRQ_to_SYS
        BL     T2TIMI_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : T3TIMIIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T3TIMI_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T3TIMI_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
T3TIMIIRQHandler:	
        IRQ_to_SYS
        BL     T3TIMI_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : HDLCIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the HDLC_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   HDLC_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
HDLCIRQHandler:	
        IRQ_to_SYS
        BL     HDLC_IRQHandler
        SYS_to_IRQ
	
#;*******************************************************************************
#;* Function Name  : USBLPIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the USBLP_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   USBLP_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
USBLPIRQHandler:	
        IRQ_to_SYS
        BL     USBLP_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : T0TOIIRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T0TOI_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T0TOI_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
T0TOIIRQHandler:	
        IRQ_to_SYS
        BL     T0TOI_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : T0OC1IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T0OC1_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T0OC1_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
T0OC1IRQHandler:	
        IRQ_to_SYS
        BL     T0OC1_IRQHandler
        SYS_to_IRQ

#;*******************************************************************************
#;* Function Name  : T0OC2IRQHandler
#;* Description    : This function used to switch to SYS mode before entering
#;                   the T0OC2_IRQHandler function located in 71x_it.c.
#;                   Then to return to IRQ mode after the
#;                   T0OC2_IRQHandler function termination.
#;* Input          : none
#;* Output         : none
#;*******************************************************************************
T0OC2IRQHandler:	
        IRQ_to_SYS
        BL     T0OC2_IRQHandler
        SYS_to_IRQ

	.ltorg
	.end

;******************* (C) COPYRIGHT 2003 STMicroelectronics *****END OF FILE****
