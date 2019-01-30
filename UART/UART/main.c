
/*
 * main.c
 *
 * Created: 23-Jan-19 10:59:21 AM
 * Author: Ranul Deepanayake
 */ 
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(){
	char *temp= "";

	uart_set(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);
	DDRB|= 0x20;
	
	while(1){
		uart_println("Hello world!");
		while(uart_available()){
			*temp= uart_read_char();
			PORTB^= 0x20; 
			uart_println(temp);
		}
		_delay_ms(250);
	}
}