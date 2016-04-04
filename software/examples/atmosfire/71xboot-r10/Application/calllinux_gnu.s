  .section .text
  .arm
  .global CALL_KERNEL

  .EQU Mode_SVC           ,     0x13
  .EQU I_Bit              ,     0x80 @ when I bit is set, IRQ is disabled
  .EQU F_Bit              ,     0x40 @ when F bit is set, FIQ is disabled	
#	void CALL_KERNEL(unsigned int address)
#       Call linux kernel image

CALL_KERNEL:

	MSR     CPSR_c, #Mode_SVC | F_Bit | I_Bit	@enter SVC with FIQ IRQ disabled
	MOV	pc, r0

	.LTORG
	.END