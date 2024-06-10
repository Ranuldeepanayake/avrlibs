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

;=============================================================================================================
;REGISTER DEFINITIONS (for our convenience).
;=============================================================================================================
.def general_register_1= r16	;	Define register with special name.

;=============================================================================================================	
;PROGRAM
;=============================================================================================================

.org 0x0000	;	Write next instruction to address 0x0000 in flash.
rjmp setup	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'.  

setup:
	rjmp loop

loop:
	ldi r16, 50
	ldi r17, 50
	cp r16, r17			;	Compare the values of two registers (what really happens is a subtraction and the manipulation of the zero flag according to the result). 
	breq if				;	Check for the zero flag after the compare and branch if the flag is set.
	rjmp loop			;	Execute this instruction if branch doesn't happen.

if:
	ldi r18, 127
	;call cp_1
	;call cpse_1
	;call cpc_1
	;call tst_1
	;call brsh_1
	;call brlo_1
	;call brpl_1
	;call brge_1
	call brlt_1
	rjmp if

cp_1:
	ldi r16, 101
	ldi r17, 100
	cp r16, r17			;	The zero flag is set if the operands are equal and cleared if the operands are not equal.
	breq return
	rjmp cp_1

return:
	ret

cpse_1:
	ldi r16, 100
	ldi r17, 100
	cpse r16, r17
	ret
	rjmp cpse_1

cpc_1:
	ldi r16, 0xE8
	ldi r17, 0x03
	ldi r18, 0xFF
	ldi r19, 0x00
	cp r16, r18
	cpc r17, r19
	brne cpc_1			;	Check for the zero flag after the compare and branch if the flag is cleared.
	ret		
	
tst_1:
	ldi r16, 10
	tst r16
	brmi tst_1			;	Check for the negative flag after the compare and branch if the flag is set.
	ret

brsh_1:					;	Branch if same or higher (unsigned).
	ldi r16, 50
	;cpi r16, 50
	;sec				;	Manually set the carry flag (just for testing). Branch instructions can be manipulated by maually setting or clearing SREG flags.
	;clc				;	Manually clear the carry flag (just for testing). Branch instructions can be manipulated by maually setting or clearing SREG flags.
	brsh brsh_1			;	Check for the carry flag after the compare and branch if the flag is cleared.
	ret
		

brlo_1:					;	Branch if lower (unsigned).
	ldi r16, -11			
	cpi r16, -10
	brlo brlo_1			;	Check for the carry flag after the compare and branch if the flag is set.
	ret

brpl_1:					;	Branch if plus.
	ldi r16, 50
	subi r16, 51
	brpl brpl_1			;	Check for the negative flag after the compare and branch if the flag is cleared.
	ret

brge_1:					;	Branch if greater or equal (signed).
	ldi r16, 10
	subi r16, 15
	brge brge_1			;	Check for the signed flag after the compare and branch if the flag is cleared.
	ret

brlt_1:					;	Branch if less than (signed).
	ldi r16, -5
	cpi r16, -1
	brlt brlt_1			;	Check for the signed flag after the compare and branch if the flag is set.
	ret