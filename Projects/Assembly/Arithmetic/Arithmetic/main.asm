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
;.def general_register_1= r16	;	Define register with special name.

;=============================================================================================================	
;PROGRAM
;=============================================================================================================

.org 0x0000	;	Write next instruction to address 0x0000 in flash.
rjmp reset	;	This is the first address, which is the reset vector. It instructs to jump to 'setup'.  

reset:							;	Labels aren't considered as instructions and don't consume CPU cycles. However, they are assigned addresses in the program space. 
		rjmp loop
;setup:
loop:
		;call add_1				;	Call the subroutine. The stack pointer is decremented and the program counter is loaded with the address of the subroutine's
								;	first instruction. The return address (address of 'rjmp loop') is stored in the the stack (RAM).
		;call add_2
		;call add_3
		;call add_4
		;call add_5
		;call add_6
		;call adc_1
		;call sub_1
		;call sub_2
		;call sub_3
		;call sbc_1
		;call multiply_8_unsigned
		;call mul_1
		;call muls_1
		;call mulsu_1
		;call b16xb8_unsigned_mul
		call b16xb16_unsigned_mul
		rjmp loop				;	Jump back to the loop (consider this as the main while loop in C). 

add_1:
		ldi r16, 2
		ldi r17, 3 
		add r16, r17
		mov r0, r16		;	Copy contents of R16 to R0.
		ret				;	Load the program counter with the return address and increment the stack pointer.

add_2:
		ldi r16, 128	;	Integers are between 0- 127 in two's complement arithmetic.
		ldi r17, 2
		add r16, r17	;	If the result of ADD is greater than 127 when an operand is equal to or less than 127, it will be a negative number. 
						;	It will also casue a signed integer overflow. The negative, overflow and half carry flags will be set.
						;	If the result of ADD is greater than 127 when an operand is equal to or greater than 128, it will be a negative number.  
						;	The negative and sign flags will be set.
		ret

add_3:
		ldi r16, 255	;	If the result of ADD is greater than 255, the carry flag will be set (the zero flag will also be set accordingly).
		ldi r17, 2
		add r16, r17
		ret

add_4:
		ldi r16, -1		;	255 in Two's complement.
		ldi r17, -1		;	255 in Two's complement.
		add r16, r17	;	Sets the cary, negative, signed and half carry flags.
		ret

add_5:
		ldi r16, 120		;	
		ldi r17, 120		;	
		add r16, r17		;	
		ldi r17, 10			;
		add r16, r17		;
		ldi r17, -5			;
		add r16, r17		;
		ret

add_6:						;	Addition of two byte variables.
		ldi r16, 0xE8		;	Low byte of 1000.
		ldi r17, 0x03		;	High byte of 1000.
		ldi r18, 0x0A		;	Low byte of 10.
		ldi r19, 0x00		;	High byte of 10.
		add r16, r18		;	Addition result will be stored in the low byte of the first number.
		adc r17, r19		;	Addition result will be stored in the high byte of the first number.
		ret

adc_1:						;	Addition of two byte variables with carry.
		ldi r16, 0xE8		;	Low byte of 1000.
		ldi r17, 0x03		;	High byte of 1000.
		ldi r18, 0xFF		;	Low byte of 255.
		ldi r19, 0x00		;	High byte of 255.
		add r16, r18		;	Addition result will be stored in the low byte of the first number.
		adc r17, r19		;	Addition result will be stored in the high byte of the first number.
		ret

sub_1:						;	Subtract without carry.
		ldi r16, 10
		ldi r17, 5
		sub r16, r17		
		ret

sub_2:
		ldi r16, 5
		ldi r17, 10
		sub r16, r17
		ret

sub_3:
		ldi r16, 150
		ldi r17, 30
		sub r16, r17
		ret

subi_1:						;	Subtract immediate.
		ldi r16, 50			
		subi r16, 10
		ret

sbc_1:						;	Subtraction of two byte variables with carry.
		ldi r16, 0xE8		;	Low byte of 1000.	
		ldi r17, 0x03		;	High byte of 1000.
		ldi r18, 0xFF		;	Low byte of 255.
		ldi r19, 0x00		;	High byte of 1000.
		sub r16, r18		;	
		sbc r17, r19		;	
		ret

multiply_8_unsigned:		;	The following three labels demonstrate assembly multiplication without hardware support.
		ldi	r16, 4			;	Load muliplicand.
		ldi r17, 3			;	Load multiplier.
		ldi r19, 8			;	Load loop counter with 8.
		lsr r17

multiply_8_unsigned_a:	
		brcc multiply_8_unsigned_b	;	Branch if carry cleared.
		add r18, r16

multiply_8_unsigned_b:
		ror r18				;	Rotate right. 
		ror	r17			
		dec r19				;	Decrement loop counter.
		brne multiply_8_unsigned_a	;	Branch if not equal.
		ret

mul_1:						;	Multiplication using hardware support (unsigned).
		ldi r16, 255
		ldi	r17, 200
		mul r16, r17
		mov r16, r0			;	The low byte of the result is stored in r0.
		mov r17, r1			;	The high byte of the result is stored in r1.
		ret

muls_1:						;	Multiplication using hardware support (signed).
		ldi r16, 255		;	-1
		ldi	r17, 200		;	-56
		muls r16, r17
		mov r16, r0			;	The low byte of the result is stored in r0.
		mov r17, r1			;	The high byte of the result is stored in r1.
		ret

mulsu_1:					;	Multiplication using hardware support (signed* unsigned).
		ldi r16, 255		;	-1
		ldi	r17, 1
		mulsu r16, r17		;	Rd is a signed integer while Rr is an unsigned integer.
		mov r16, r0			;	The low byte of the result is stored in r0.
		mov r17, r1			;	The high byte of the result is stored in r1.
		ret

/*
.def reg_result_l= r0
.def reg_result_h= r1
.def reg_multiplicand_l= r2
.def reg_multiplicand_h= r3 
.def reg_multiplier= r4
.def product_0l= r5 
.def product_1= r6
.def product_2h= r7
.def temp= r16
.equ multiplicand= 1000
.equ multiplier= 100
 
b16xb8_unsigned_mul:						;	16 bit* 8 bit multiplication (unsigned).
		ldi temp, LOW(multiplicand)			;	16 bit value lower register.
		mov reg_multiplicand_l, temp
		ldi temp, HIGH(multiplicand)		;	16 bit value upper register.
		mov reg_multiplicand_h, temp
		ldi temp, multiplier				;	8 bit value register.
		mov reg_multiplier, temp
		
		mul reg_multiplicand_l, reg_multiplier
		mov product_0l, reg_result_l
		mov product_1, reg_result_h

		mul reg_multiplicand_h, reg_multiplier
		mov product_2h, reg_result_h

		add product_1, reg_result_l
		brcc b16xb8_unsigned_mul_ret		;	Branch if carry is cleared.
		inc product_2h						;	Increment product high byte if carry is set.
		ret

b16xb8_unsigned_mul_ret:
		ret
*/

.def reg_result_l= r0
.def reg_result_h= r1
.def reg_multiplicand_l= r2
.def reg_multiplicand_h= r3 
.def reg_multiplier_l= r4
.def reg_multiplier_h= r5
.def product_0= r6 
.def product_1= r7
.def product_2= r8
.def product_3= r9
.def temp= r16
.equ multiplicand= 1000
.equ multiplier= 1000
;	Result= 0x000F4240= 1000000

b16xb16_unsigned_mul:
		ldi temp, LOW(multiplicand)			;	16 bit value lower register.
		mov reg_multiplicand_l, temp
		ldi temp, HIGH(multiplicand)		;	16 bit value upper register.
		mov reg_multiplicand_h, temp
		ldi temp, LOW(multiplier)			;	16 bit value upper register.
		mov reg_multiplier_l, temp
		ldi temp, HIGH(multiplier)			;	16 bit value upper register.
		mov reg_multiplier_h, temp
		clr	temp							;	Clear register for carry operations.

		mul reg_multiplicand_l, reg_multiplier_l	;	Multiply LSBs.
		mov product_0, reg_result_l
		mov product_1, reg_result_h 

		mul reg_multiplicand_h, reg_multiplier_h	;	Multiply MSBs.
		mov product_2, reg_result_l
		mov product_3, reg_result_h

		mul reg_multiplicand_h, reg_multiplier_l	;	Multiply NH with ML.
		add product_1, reg_result_l
		adc product_2, reg_result_h
		adc product_3, temp

		mul reg_multiplicand_l, reg_multiplier_h	;	Multiply NL with MH. 
		add product_1, reg_result_l
		adc product_2, reg_result_h
		adc product_3, temp
		ret 