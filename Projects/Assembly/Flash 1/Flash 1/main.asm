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

;.org 0x1F80	;	Write next instruction to address 0x0000 in flash.
.org 0x0000
rjmp reset	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'. ***Reset vector.  

reset:
		ldi r16, 0xFF
		out DDRB, r16
		ldi r16, 0x00
		out PORTB, r16
		;jmp led_on		;	Turn LED off. Not jumping tp 0x1F80 increments the number of CPU cycles.
						;	My conclusion is that the program counter iterates through program memory (flash) addresses.

.org 0x1F80
led_on:
		ldi r16, 0x01
		;eor r16, r16	;	Zero flag will be set.
		out PORTB, r16

		eor r16, r16
		out PORTB, r16

		ldi r16, 0x20
		out PORTB, r16