/*
 * usart328P.h
 *
 * Created: 30-Oct-18 6:54:33 PM
 * Author: Ranul Deepanayake.
 * UART library for the ATmega328P. Supports character and string transmission, character reception, selectable baud rate and reception error detection.
 * Supports 5- 8 bit data frames.
 */ 


#ifndef USART328P_H_
#define USART328P_H_

//Includes.
#include <avr/io.h>
#include <avr/interrupt.h>

//Attributes definitions.
#ifndef F_CPU
#define F_CPU 16000000
#endif
#define UART_BAUD_RATE(BAUD_RATE) (((F_CPU)/(BAUD_RATE* 16UL))- 1)
#define UART_NULL_CHARACTER 0x00
#define UART_CARRIAGE_RETURN 0x0D
#define UART_NEW_LINE 0x0A
#define UART_RX_COMPLETE 0x80
#define UART_TX_COMPLETE 0x40
#define UART_DATA_REGISTER_EMPTY 0x20
#define UART_RX_ENABLE 0x10
#define UART_TX_ENABLE 0x08
#define UART_2X_MODE 0x02
#define UART_ASYNCHRONOUS_MODE 0x00
#define UART_DATA_SIZE_5 UCSR0C|= 0x00;
#define UART_DATA_SIZE_6 UCSR0C|= 0x02;
#define UART_DATA_SIZE_7 UCSR0C|= 0x04;
#define UART_DATA_SIZE_8 UCSR0C|= 0x06;
//#define UART_DATA_SIZE_9 UCSR0C|= 0x06; UCSR0B|= 0x04;
#define UART_PARITY_NONE 0x00 
#define UART_PARITY_EVEN 0x20
#define UART_PARITY_ODD 0x30
#define UART_STOP_BITS_1 0x00
#define UART_STOP_BITS_2 0x08

//Error and status codes.
#define UART_FRAME_ERROR 0x10
#define UART_DATA_OVERRUN_ERROR 0x08
#define UART_PARITY_ERROR 0x04
#define UART_OK 0x00

//Functions.
//Set up the UART peripheral.
void uart_set(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits);
//Transmit a single character on the TX line.
void uart_send_char(uint8_t data);
//Send a string without carriage return and newline.
void uart_print(char *string_pointer);	
//Send a string with carriage return and newline.
void uart_println(char *string_pointer);
//Check for unread received characters on the RX line.
uint8_t uart_available();
//Read a character received on the RX line.
uint8_t uart_read_char();
//Wait for till a character is received on the RX line.
char uart_read_char_wait();	
//Check for a frame error on reception.
uint8_t uart_frame_error();
//Check for a data overrun error on reception.
uint8_t uart_data_overrun_error();
//Check for a parity on reception.
uint8_t uart_parity_error();

//Example implementation.
/*
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(){
	char *temp= "";

	uart_set(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);	//Set up UART peripheral. 
	DDRB|= 0x20; //LED.
	
	while(1){
		uart_println("Hello world!");	//Transmit characters.
		while(uart_available()){	//Check for received and unread characters.
			*temp= uart_read_char();	//Read character.
			PORTB^= 0x20;	//LED toggle according to user input.
			uart_println(temp);	//Loopback.
		}
		_delay_ms(250);
	}
}
*/

#endif /* USART328P_H_ */