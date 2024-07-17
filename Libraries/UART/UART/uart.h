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

//CPU frequency definition.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define UART_BAUD_RATE(BAUD_RATE) (((F_CPU)/(BAUD_RATE* 16UL))- 1)
#define UART_NULL_CHARACTER 0x00
#define UART_CARRIAGE_RETURN 0x0D
#define UART_NEW_LINE 0x0A

#define UART_RX_COMPLETE 0x80
#define UART_TX_COMPLETE 0x40

//Various UART modes.
#define UART_TX_ENABLE 0x08
#define UART_RX_ENABLE 0x10
#define UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE 0x20
#define UART_TX_INTERRUPT_ENABLE 0x40
#define UART_RX_INTERRUPT_ENABLE 0x80
#define UART_2X_MODE 0x02
#define UART_ASYNCHRONOUS_MODE 0x00
#define UART_DATA_SIZE_5 0x00;
#define UART_DATA_SIZE_6 0x02;
#define UART_DATA_SIZE_7 0x04;
#define UART_DATA_SIZE_8 0x06;
//#define UART_DATA_SIZE_9 UCSR0C|= 0x06; UCSR0B|= 0x04;
//LEDs.
#ifndef UART_TX_LED
#define UART_TX_LED 0x02
#endif

#ifndef UART_RX_LED
#define UART_RX_LED 0x01
#endif

//Flags.
#define UART_DATA_REGISTER_EMPTY 0x20

//Parity and stop bits selection.
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
#define UART_TX_BUFFER_OK 1
#define UART_RX_BUFFER_OK 1
#define UART_TX_BUFFER_FULL 2
#define UART_RX_BUFFER_FULL 3
#define UART_TX_BUFFER_EMPTY 4
#define UART_RX_BUFFER_EMPTY 5
#define UART_UNDEFINED_CONDITION 100

#define UART_BUFFER_TYPE_TX 0
#define UART_BUFFER_TYPE_RX 1
#define UART_TX_BUFFER_SIZE 64
#define UART_RX_BUFFER_SIZE 64

/*
Function definitions.
*/
//Set up the UART peripheral.
void uartSet(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits);
void uartSetLed(uint8_t toggle);
//Transmit a single character on the Tx line.
void uartSendChar(char data);
//Send a string without carriage return and newline.
void uartPrint(char *string_pointer);	
//Send a string with carriage return and newline.
void uartPrintLn(char *string_pointer);
//Pushes a received character into the Rx buffer.
uint8_t uartBufferPush(uint8_t buffer_type, char data);
//Pops a received character from the Rx buffer.
uint8_t uartBufferPop(uint8_t buffer_type, char *data);
//Returns the number of unread characters in the Rx buffer.
uint8_t uartAvailable(uint8_t buffer_type);
uint8_t uartCount(uint8_t buffer_type);
uint8_t uartIsFull(uint8_t buffer_type);
uint8_t uartPeek(uint8_t buffer_type, int *data);
//Read a character in the Rx buffer.
char uartRead();
//Wait till a character is received in the receive register.
char uartReadCharWait();
//Check for a frame error on reception.
uint8_t uartFrameError();
//Check for a data overrun error on reception.
uint8_t uartDataOverrunError();
//Check for a parity on reception.
uint8_t uartParityError();


//Example implementation.
/*
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(){
	uartSet(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);	//Set up UART peripheral. 
	
	char buffer[7];
	uint8_t position= 0;
	uint8_t complete= 0;
	
	while(1){
		uartPrintLn("Sending message...");
		
		while(uartAvailable()> 0 && complete== 0){
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
				uartSendChar(buffer[i]);
			}
			complete= 0;
		}
		
		uartPrintLn("\n");
		_delay_ms(250);
	}
}
*/

#endif /* USART328P_H_ */