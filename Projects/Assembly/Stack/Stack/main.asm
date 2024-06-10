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
.equ a= 2500
.equ b= 1000

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
	ldi r16, LOW(RAMEND)		;	Set up the stack pointer.
	out spl, r16
	ldi r16, HIGH(RAMEND)
	out sph, r16
	;rjmp loop

init:
	ldi r17, 5

loop:
	ldi r16, 10
	add r16, r17
	rcall function_2
	clr r16
	rcall function_1			;	'rcall' consumes one less cycle than 'call'. **However it can address only k+ 2K or k- 2K word addresses.
	rjmp loop

.org 0x05E8						;	Position code from here after the specified flash address.

function_1:						;	3 bytes will be pushed in to the stack (2 byte return address and 1 byte variable in r17).
	push r17					;	The value which is in r17 is pushed in to the stack. 
	ldi r17, 0
	pop r17						;	The value in the stack corresponding to r17 is popped back into r17.
	ret

function_2:
	push r16
	push r17

	ldi r16, LOW(a)				;	16 bit integer (2500).
	ldi r17, HIGH(a)
	ldi r18, LOW(b)				;	16 bit integer (2500).
	ldi r19, HIGH(b)

	rcall function_3				;	Sub-routine call inside sub-routine.

	add r16, r18
	adc r17, r19
	movw r1:r0, r17:r16			;	Copy register word Rr+ 1:Rr to Rd+ 1:Rd. 

	;clr r19
	;clr r18
	pop r17
	pop r16
	ret

function_3:
	push r16
	push r17
	push r18
	push r19

	clr r16
	clr r17
	clr r18
	clr r19

	pop r19
	pop r18
	pop r17
	pop r16

	ret
	
