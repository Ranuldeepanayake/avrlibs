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
;MACROS (values are substituted only during assembly and can't be changed during runtime and also, a macro just includes its whole code in the source).
;=============================================================================================================
.macro load_16bit			;	Loads a 16 bit value to a pair of registers.
ldi @0, LOW(@2)
ldi @1, HIGH(@2)
.endmacro

.macro subtraction_16bit	;	Subtracts two 16 bit numbers.
sub @0, @1
sbc @2, @3
movw r3:r2, @2:@0
.endmacro

.macro set_stack_pointer	;	Sets up the stack pointer.	
ldi r16, LOW(RAMEND)
out spl, r16
ldi r16, HIGH(RAMEND)
out sph, r16
.endmacro

;=============================================================================================================	
;PROGRAM
;=============================================================================================================

.org 0x0000	;	Write next instruction to address 0x0000 in flash.
rjmp setup	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'.  

setup:
	set_stack_pointer		;	Call macro to set up the stack pointer.
	rjmp loop

loop:
	load_16bit r16, r17, a
	load_16bit r18, r19, b
	subtraction_16bit r16, r18, r17, r19
	rjmp loop
	

