/*
 * uart328P.h
 *
 * Created: 30-Oct-18 6:54:33 PM
 * Author: Ranul Deepanayake.
 * UART library for the ATmega328P. Supports character transmission, reception, selectable baud rates, reception error detection and activity indication.
 * Runs in the asynchronous master 2x mode.
 * Supports baud rates of 9600- 500000 (other values may work but have not been tested).
 * Circular buffer and interrupt based transmission and reception. Tx and Rx buffers sizes can be set individually. 
 * Supports 5- 8 bit data frames. Development for 9 bit data frames is pending.
 * Supports parities of none, even and odd.
 * Supports one and two stop bits.
 * Supports frame error, overrun error and parity error detection with free running error counters.
 * Supports Tx/Rx activity indicator LEDs which use PORTD. Default pins; Rx- PORTD2 and Tx- PORTD3.
 */ 

#ifndef USART328P_H_
#define USART328P_H_

//CPU frequency definition.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
//Tx LED.
#ifndef UART_TX_LED
#define UART_TX_LED 0x04
#endif
//Rx LED.
#ifndef UART_RX_LED
#define UART_RX_LED 0x08
#endif
//Calculations.
#define UART_BAUD_RATE(BAUD_RATE) (((F_CPU)/(BAUD_RATE* 8UL))- 1)
//Buffer modifiers.
#define UART_BUFFER_TYPE_TX 0
#define UART_BUFFER_TYPE_RX 1
#define UART_TX_BUFFER_SIZE 512
#define UART_RX_BUFFER_SIZE 4
//Various UART operational modes.
#define UART_TX_ENABLE 0x08
#define UART_RX_ENABLE 0x10
#define UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE 0x20
#define UART_TX_INTERRUPT_ENABLE 0x40
#define UART_RX_INTERRUPT_ENABLE 0x80
#define UART_2X_MODE 0x02
#define UART_ASYNCHRONOUS_MODE 0x00
#define UART_DATA_SIZE_5 0x00
#define UART_DATA_SIZE_6 0x02
#define UART_DATA_SIZE_7 0x04
#define UART_DATA_SIZE_8 0x06
#define UART_PARITY_NONE 0x00
#define UART_PARITY_EVEN 0x20
#define UART_PARITY_ODD 0x30
#define UART_STOP_BITS_1 0x00
#define UART_STOP_BITS_2 0x08
//#define UART_DATA_SIZE_9 0x08 //Need to configure two registers. This value is for differentiation only, and is not the value of either of the registers.
//Status and error flags.
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
#define UART_RX_COMPLETE 0x80
#define UART_TX_COMPLETE 0x40
#define UART_DATA_REGISTER_EMPTY 0x20
#define UART_UNDEFINED_CONDITION 100
//ASCII characters.
#define UART_NULL_CHARACTER 0x00
#define UART_CARRIAGE_RETURN 0x0D
#define UART_NEW_LINE 0x0A

/*
Function definitions.
*/
//Set up the UART peripheral.
void uartSet(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits);
//Set up Tx and Rx LEDs. Uses PORTD.
void uartSetLed(uint8_t toggle);
//Transmit a string without carriage return and newline.
uint8_t uartPrint(char *string_pointer);	
//Transmit a string with carriage return and newline.
uint8_t uartPrintLn(char *string_pointer);
//Push a character into the the selected buffer.
uint8_t uartBufferPush(uint8_t buffer_type, char data);
//Pop a character from the the selected buffer.
uint8_t uartBufferPop(uint8_t buffer_type, char *data);
//Return whether unread characters are present in the selected buffer.
uint8_t uartAvailable(uint8_t buffer_type);
//Return the number of unread characters in the selected buffer.
uint8_t uartCount(uint8_t buffer_type);
//Return whether the selected buffer is full.
uint8_t uartIsFull(uint8_t buffer_type);
//Read the last character from the selected buffer.
uint8_t uartPeek(uint8_t buffer_type, int *data);
//Read a character from the Rx buffer.
char uartRead();
//Check for a frame error on reception.
uint8_t uartFrameError();
//Return the frame error count.
uint8_t uartGetFrameErrorCount();
//Check for a data overrun error on reception.
uint8_t uartOverrunError();
//Return the overrun error count.
uint8_t uartGetOverrunErrorCount();
//Check for a parity on reception.
uint8_t uartParityError();
//Return the parity error count.
uint8_t uartGetParityErrorCount();
//(Deprecated) Transmit a single character.
void uartSendChar(char data);
//(Deprecated) Wait till a character is received in the receive register.
char uartReadCharWait();

/*
Example implementation.

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

int main(){
	uartSet(UART_BAUD_RATE(9600), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);	//Set up UART peripheral. 
	
	char buffer[7];
	uint8_t position= 0;
	uint8_t complete= 0;
	
	while(1){
		uartPrintLn("Sending message...");
		
		while(uartAvailable() && complete== 0){
			char temp= uartRead();
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