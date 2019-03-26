/*
 * usart328P.h
 *
 * Created: 30-Oct-18 6:54:33 PM
 * Author: Ranul Deepanayake.
 * UART library for the ATmega328P. Supports character and string transmission, reception, character reception, selectable baud rate and reception error detection.
 * Has a re-sizable RX buffer.
 * Supports 5- 8 bit data frames.
 */ 


#ifndef USART328P_H_
#define USART328P_H_

//Includes.
#include <avr/io.h>
#include <avr/interrupt.h>

//Attributes definitions.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#define UART_BAUD_RATE(BAUD_RATE) (((F_CPU)/(BAUD_RATE* 16UL))- 1)
#define UART_NULL_CHARACTER 0x00
#define UART_CARRIAGE_RETURN 0x0D
#define UART_NEW_LINE 0x0A
#define UART_RX_COMPLETE 0x80
#define UART_TX_COMPLETE 0x40
#define UART_DATA_REGISTER_EMPTY 0x20
#define UART_RX_INTERRUPT_ENABLE 0x80
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

#define UART_RX_BUFFER_SIZE 64	//Change according to the required RX buffer size.

//Error and status codes.
#define UART_FRAME_ERROR 0x10
#define UART_DATA_OVERRUN_ERROR 0x08
#define UART_PARITY_ERROR 0x04
#define UART_OK 0x00

//External variables.
extern struct circular_buffer{
	char buffer[UART_RX_BUFFER_SIZE];
	uint8_t head;
	uint8_t tail;
}rx_buffer;

//Functions.
//Set up the UART peripheral.
void uart_set(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits);
//Transmit a single character on the TX line.
void uart_send_char(uint8_t data);
//Send a string without carriage return and newline.
void uart_print(char *string_pointer);	
//Send a string with carriage return and newline.
void uart_println(char *string_pointer);
//Pushes a received character into the RX buffer.
void uart_rx_buffer_push(struct circular_buffer *buff, uint8_t data);
//Pops a received character from the RX buffer.
char uart_rx_buffer_pop(struct circular_buffer *buff);
//Returns the number of unread characters in the RX buffer.
uint16_t uart_available();
//Read a character in the RX buffer.
char uart_read();
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
	uart_set(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);	//Set up UART peripheral. 
	
	char buffer[7];
	uint8_t position= 0;
	uint8_t complete= 0;
	
	while(1){
		uart_println("Sending message...");
		
		while(uart_available()> 0 && complete== 0){
			char temp= uart_read();
			if(temp== '\r'){
				buffer[position]= 0;
				position= 0;
				complete= 1;
			}else if(temp== '\n'){
				//Ignore the newline character.
			}else{
				if(position<= 6){	//Position limit.
					buffer[position++]= temp;
				}
			}
		}
		
		if(complete== 1){
			for(uint8_t i= 0; i< 6; i++){
				uart_send_char(buffer[i]);
			}
			complete= 0;
		}
		
		uart_println("\n");
		_delay_ms(250);
	}
}
*/

#endif /* USART328P_H_ */