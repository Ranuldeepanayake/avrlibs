	.file	"dht11.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.section	.text.dht11_set,"ax",@progbits
.global	dht11_set
	.type	dht11_set, @function
dht11_set:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ret
	.size	dht11_set, .-dht11_set
	.section	.text.dht11_get_raw_data,"ax",@progbits
.global	dht11_get_raw_data
	.type	dht11_get_raw_data, @function
dht11_get_raw_data:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0xa,7
	cbi 0xb,7
	ldi r18,lo8(57599)
	ldi r24,hi8(57599)
	ldi r25,hlo8(57599)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	sbi 0xb,7
	ldi r18,lo8(-43)
1:	dec r18
	brne 1b
	nop
	ret
	.size	dht11_get_raw_data, .-dht11_get_raw_data
	.section	.text.dht11_get_temperature,"ax",@progbits
.global	dht11_get_temperature
	.type	dht11_get_temperature, @function
dht11_get_temperature:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r22,0
	ldi r23,0
	movw r24,r22
	ret
	.size	dht11_get_temperature, .-dht11_get_temperature
	.section	.text.dht11_get_humidity,"ax",@progbits
.global	dht11_get_humidity
	.type	dht11_get_humidity, @function
dht11_get_humidity:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r22,0
	ldi r23,0
	movw r24,r22
	ret
	.size	dht11_get_humidity, .-dht11_get_humidity
	.section	.text.dht11_get_heat_index,"ax",@progbits
.global	dht11_get_heat_index
	.type	dht11_get_heat_index, @function
dht11_get_heat_index:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r22,0
	ldi r23,0
	movw r24,r22
	ret
	.size	dht11_get_heat_index, .-dht11_get_heat_index
	.ident	"GCC: (GNU) 4.9.2"
