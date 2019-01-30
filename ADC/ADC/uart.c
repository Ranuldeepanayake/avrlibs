/*
 * Usart328P.c
 *
 * Created: 30-Oct-18 6:53:44 PM
 * Author : Ranul Deepanayake
 */ 
#include "uart.h"

/*
Initializes the UART peripheral. 
A baud rate between 9600- 57600 is supported.
Frames of 5- 8 data bits is supported.
*/
void uart_set(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits){
	//Set the USART registers.
	UBRR0H= baud_rate>> 8;	//Set the baud rate high byte.
	UBRR0L= baud_rate;	//Set the baud rate low byte.
	UCSR0B= (UART_RX_ENABLE | UART_TX_ENABLE);		//Enable TX and RX.
	UCSR0C= (UART_ASYNCHRONOUS_MODE | parity | stop_bits);	//Set mode, parity and stop bits.
	
	//Set frame size.
	switch(data_bits){
		case 5:
			UART_DATA_SIZE_5
			break; 
		case 6:
			UART_DATA_SIZE_6
			break;
		case 7:
			UART_DATA_SIZE_7
			break;
		case 8:
			UART_DATA_SIZE_8
			break;
		case 9:
			//UART_DATA_SIZE_9
			break;
	}	
}

/*
Transmits a single character on the TX line.
*/
void uart_send_char(uint8_t data){
	//Sends a single character.
	while(!(UCSR0A & UART_DATA_REGISTER_EMPTY)); //Check the status of UDRE0
	UDR0= data;	//Write data to the register.
}

/*
Transmits a string of characters on the TX line. Carriage return and newline aren't sent.
*/
void uart_print(char *string_pointer){
	while(*string_pointer!= UART_NULL_CHARACTER){ //Check for the null character in the string.
		uart_send_char((uint8_t) *(string_pointer++)); //Sends the value in the memory location.
	}
}

/*
Transmits a string of characters on the TX line. Carriage return and newline are sent.
*/
void uart_println(char *string_pointer){
	while(*string_pointer!= UART_NULL_CHARACTER){ //Check for the null character in the string.
		uart_send_char(*(string_pointer++)); //Sends the value in the memory location.
	}
	uart_send_char(UART_CARRIAGE_RETURN);	//Print carriage return.
	uart_send_char(UART_NEW_LINE); //Print newline.
}

/*
Waits in a loop till a character is received on the RX buffer.
*/
char uart_read_char_wait(){
	//Waits till a character is received in a while loop.
	while(!(UCSR0A & UART_RX_COMPLETE)); //Check the status of RXC0.
	return UDR0;
}

/*
Checks whether an unread character is in the RX buffer.
*/
uint8_t uart_available(){
	return (UCSR0A & UART_RX_COMPLETE);
}

/*
Returns a single char received on the RX buffer. Used in conjunction with 'uart_available()'.
*/
uint8_t uart_read_char(){
	return UDR0;
}

/*
Detects a frame error in data reception.
*/
uint8_t uart_frame_error(){
	if(UCSR0A & UART_FRAME_ERROR){
		return UART_FRAME_ERROR;
	}else{
		return UART_OK;
	}
}

/*
Detects a data overrun error in data reception.
*/
uint8_t uart_data_overrun_error(){
	if(UCSR0A & UART_DATA_OVERRUN_ERROR){
		return UART_DATA_OVERRUN_ERROR;
	}else{
		return UART_OK;
	}
}

/*
Detects a parity error in data reception.
*/
uint8_t uart_parity_error(){
	if(UCSR0A & UART_PARITY_ERROR){
		return UART_PARITY_ERROR;
	}else{
		return UART_OK;
	}
}

