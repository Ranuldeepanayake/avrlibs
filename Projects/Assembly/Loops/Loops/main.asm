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
.equ value_1= 2500
.equ value_2= 0

;=============================================================================================================
;REGISTER DEFINITIONS (for our convenience).
;=============================================================================================================
.def general_register_1= r16	;	Define register with special name.

;=============================================================================================================	
;MACROS (values are substituted only during assembly and can't be changed during runtime and also, a macro just includes its whole code in the source).
;=============================================================================================================
.macro set_stack_pointer	;	Sets up the stack pointer.	
ldi r16, LOW(RAMEND)
out spl, r16
ldi r16, HIGH(RAMEND)
out sph, r16
.endmacro

.macro load_16bit			;	Loads a 16 bit value to a pair of registers.
ldi @0, LOW(@2)
ldi @1, HIGH(@2)
.endmacro

;=============================================================================================================	
;PROGRAM
;=============================================================================================================

.org 0x0000	;	Write next instruction to address 0x0000 in flash.
rjmp setup	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'.  

setup:
	set_stack_pointer
	rjmp loop

	
loop:					;	A simple for loop in the main loop.
	ldi r16, 0
	ldi r17, 0
	;call while_routine_1
	;rcall do_while_routine_1 
	rcall for_loop_routine_3

for_loop_1:				
	inc r17				;	Do work here. Do work befor the incrementer to use '0'.
	inc r16				;	Incrementer.
	cpi r16, 5			;	Break if r16== 4.
	brne for_loop_1

	ldi r16, 5
	ldi r17, 0
for_loop_2:				;	Down counting for loop.
	inc r17
	dec r16
	breq loop
	rjmp for_loop_2

;------------------------------------------------------------------------------
;While loop. Compares an incremented (by 500) value to 2500.
;------------------------------------------------------------------------------
while_routine_1:
	push r16					;	Save previous registers.
	push r17 
	ldi r16, LOW(500)			;	Load incrementer value to a lower register pair.
	mov r10, r16
	ldi r16, HIGH(500)
	mov r11, r16
	ldi r16, LOW(value_1)		;	Load comparator value to r16 and r17.
	ldi r17, HIGH(value_1)
	load_16bit r18, r19, value_2;	Load register pair with initial value.
	ldi r20, 0

while_loop_1:
	cp r16, r18					;	Compare value with comparator.
	cpc r17, r19
	breq while_routine_1_end	;	Break if equal.
	add r18, r10				;	Increment value.
	adc r19, r11 
	inc r20						;	For counting the number of cycles.
	rjmp while_loop_1			;	Repeat.

while_routine_1_end:
	pop r17						; Pop previous registers.
	pop r16
	ret							; Return.
;------------------------------------------------------------------------------
;Do-while loop.
;------------------------------------------------------------------------------

do_while_routine_1:
	ldi r16, 6
	ldi r17, 0

do_while_loop_1:
	inc r17
	cpi r16, 5
	brlo do_while_loop_1		;	Branch if lower than.
	ret

;------------------------------------------------------------------------------
;For loop.
;------------------------------------------------------------------------------
for_loop_routine_3:
	in r16, sreg				;	Preserve SREG.
	push r16
	ldi r16, 0					;	Incrementer.
	ldi r17, 0
	
for_loop_3:	
	cpi r16, 5
	brsh for_loop3_end			;	Branch if same or higher.
	mov r17, r16				;	Do work.
	inc r16						;	Increment incrementer.
	rjmp for_loop_3

for_loop3_end:
	pop r16						;	Replace SREG.
	out sreg, r16
	ret
