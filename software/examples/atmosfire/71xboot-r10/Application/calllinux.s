;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    AREA    asm2,CODE,READONLY
Mode_SVC           EQU     0x13
I_Bit              EQU     0x80 ; when I bit is set, IRQ is disabled
F_Bit              EQU     0x40 ; when F bit is set, FIQ is disabled	
;	void CALL_KERNEL(unsigned int address)
;		Call linux kernel image

	EXPORT	CALL_KERNEL

CALL_KERNEL

	MSR     CPSR_c, #Mode_SVC:OR:F_Bit:OR:I_Bit	;enter SVC with FIQ IRQ disabled
	MOV	pc, r0

	LTORG
	END