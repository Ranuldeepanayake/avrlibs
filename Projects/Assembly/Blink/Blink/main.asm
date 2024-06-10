;
; Blink.asm
;
; Created: 05-Dec-18 1:19:54 PM
; Author : Ranul Deepanayake
; MCU	 : ATmega328P

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

setup:					;	These are known as labels. They are also assigned an address in program space. 
		ldi r16, 0x20	;	Load r16 with 0x20.
		out DDRB, r16	;	Load register DDRB with content of r16 to set output pin.	

led_on:
		sbi PORTB, 5	;	Set bit 5 in PORTB.
		
led_off:
		cbi PORTB, 5	;	Clear bit 7 in PORTB.s
		rjmp led_on		;	Jump back to 'led_on' 	

