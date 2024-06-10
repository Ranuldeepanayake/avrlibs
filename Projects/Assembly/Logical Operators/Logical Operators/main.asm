;=============================================================================================================
;INCLUDES (including the register definition file etc).
;=============================================================================================================
;.include "m328p.def"

;=============================================================================================================
;PROCESSOR TYPE (helps the assembler check for incompatible instructions).
;=============================================================================================================
.device atmega328p

;=============================================================================================================
;CONSTANTS (label made for our convenience).
;=============================================================================================================
.equ cpu_frequency= 16000000	;	16MHz
.equ led= 0x20					;	LED pin on PORTB
.equ button_on= 0x20

;=============================================================================================================
;REGISTER DEFINITIONS (for our convenience).
;=============================================================================================================
.def general_register_1= r16	;	Define register with special name.

;=============================================================================================================	
;PROGRAM
;=============================================================================================================

.org 0x0000	;	Write next instruction to address 0x0000 in flash.
rjmp setup	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'.  

setup:						;	Set up the stack pointer and other necessities.
		ldi r16, 0xFF		;	The low byte of the highest RAM address. LOW(RAMEND).
		out SPL, r16		
		ldi r16, 0x08		;	The low byte of the highest RAM address. HIGH(RAMEND).
		out SPH, r16
		rjmp loop

loop:
		;call andi_1
		;call and_1
		;call ori_1
		;call or_1
		;call eor_1
		;call lsr_1
		;call lsl_1
		;call test_1
		;call test_2
		call test_3
		rjmp loop

andi_1:						;	Logical AND between a register and a constant.		
		ldi r16, 0x20		;	
		andi r16, 0x20
		sbrs r16, 5			;	Skip next instruction if bit in register is set.
		ret
		rjmp andi_1

and_1:
		ldi r16, button_on	;	Logical AND between two registers.
		ldi r17, button_on
		and r16, r17
		brne and_1			;	Branch if the zero flag is cleared.
		ret

ori_1:
		ldi r16, 0x01
		ori r16, 0x20
		cpi r16, 0x21
		breq ori_1
		ret
		
or_1:
		ldi r16, 0x01
		ldi r17, button_on
		or r16, r17
		sbrs r16, 5
		ret
		rjmp or_1

eor_1:
		ldi r16, 0x20
		eor r16, r16
		breq eor_1
		ret

lsr_1:
		ldi r16, 0b01110111
		lsr r16
		ret

lsl_1:
		ldi r16, 0b01111111
		lsl r16
		ret

test_1:
		ldi r16, 0b01111111
		ldi r17, 0b01111111
		add r16, r17
		ret

test_2:
		ldi r16, 0b01111111		;	127
		ldi r17, 0b11111111		;	-1
		add r16, r17
		ret

test_3:
		ldi r16, 0b11111111		;	127
		ldi r17, 0b11111111		;	-1
		add r16, r17
		ret

		