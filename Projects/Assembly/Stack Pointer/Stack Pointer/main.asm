; Subroutines are the equivalent of functions in C. 

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

setup:							;	Initialize the stack pointer.
		ldi r16, low(RAMEND)	;	Load the low address of RAMEND to r16 (the stack pointer address is configurable by the programmer).
		out spl, r16			;	Load the value of r16 to the stack pointer low address.
		ldi r16, high(RAMEND)	;	Load the high address of RAMEND to r16.
		out sph, r16			;	Load the value of r16 to the stack pointer high address.
		
		ldi r16, 0x20			
		out DDRB, r16			;	Set LED.

		ldi r16, 0x20
		out PORTB, r16			;	Turn LED on.
		rcall delay				;	Call subroutine.
		ldi r16, 0x00		
		out PORTB, r16			;	Turn off LED.
		rcall delay				;	Call subroutine.
		
delay:							;	delay subroutine
		ldi r17, 0xFF
loop:
		dec r17					;	Decrements '1' from a register and stores to the same register.
		brne loop	
		ret				

		 


