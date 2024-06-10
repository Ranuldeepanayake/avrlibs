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
rjmp setup	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'. 'rjmp' is used to jump to anywhere within 2KB.
			;	Use 'jmp' to jump to anywhere within 64KB.

setup:
		ldi r16, led
		//ldi r17, led	
		out DDRB, r16
		clr r24			;	Clear register 'r24'
		clr r25			;	Clear register 'r25'
		clr r26
		clr r27

led_on:
		out PORTB, r16	;	Turn LED on.

led_delay_loop_inner:
		adiw r26, 1		;	Add (Add Immediate to Word) '1' to the value of the register 26 (low register), 27 (high register) pair.
		brne led_delay_loop_inner	;	Jump back to start of loop if zero flag is not set (zero flag gets set when the register pair overflows).

led_delay_loop_outer:
		adiw r24, 1		;	Add (Add Immediate to Word) '1' to the value of the register 24 (low register), 25 (high register) pair.
		cpi r24, 0x10		;	Check if r25 has the value '39'. 
		sbci r25, 0x27
		brlo led_off	;	Jump back to start of loop if carry flag is not set (zero flag gets set when the register pair overflows). **************

led_off:
		clr r24
		clr r25
		clr r26
		clr r27
		cbi PORTB, 5	; Turn LED off.
		rjmp led_on		; Jump to 'led_off'.



		



