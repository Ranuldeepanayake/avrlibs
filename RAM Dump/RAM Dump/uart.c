/*
 * Usart328P.c
 *
 * Created: 30-Oct-18 6:53:44 PM
 * Author : Ranul Deepanayake
 */ 
#include "uart.h"

struct circular_buffer rx_buffer;	//The RX buffer.

//The ISR catches received characters and pushes them into the RX buffer.
ISR(USART_RX_vect){	
	uart_rx_buffer_push(&rx_buffer, UDR0);
}

/*
Initializes the UART peripheral. 
A baud rate between 9600- 57600 is supported.
Frames of 5- 8 data bits is supported.
*/
void uart_set(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits){
	//Set the USART registers.
	UBRR0H= baud_rate>> 8;	//Set the baud rate high byte.
	UBRR0L= baud_rate;	//Set the baud rate low byte.
	UCSR0B= (UART_RX_INTERRUPT_ENABLE | UART_RX_ENABLE | UART_TX_ENABLE);		//Enable the RX interrupt, TX and RX.
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
	
	//Set up the RX buffer.
	rx_buffer.head= 0;
	rx_buffer.tail= 0;
	
	sei();	//Set global interrupts.
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
Pushes a received character into the RX buffer.
Called inside the USART_RX ISR. 
*/
void uart_rx_buffer_push(struct circular_buffer *buff, uint8_t data){
	uint8_t next= 0;
	next= buff-> head+ 1;
	
	if(next>= UART_RX_BUFFER_SIZE){	//Overwrite the first (oldest) element when the buffer is full (only if it has been read though. See the below function).
		next= 0;
	}
	
	if(next== buff-> tail){}//Discard write if buffer hasn't been read (popped).
	else{
		buff-> buffer[buff-> head]= data;	//Insert data into the buffer at the current head position.
		buff-> head= next;	//Increment the head position by one.
	}
}

/*
Pops a received character from the RX buffer.
*/
char uart_rx_buffer_pop(struct circular_buffer *buff){
	uint8_t next= 0;
	char data= 0;

	if(buff-> head== buff-> tail){}	//If head== tail, the buffer is empty and therefore shouldn't be read.
	else{
		next= buff-> tail+ 1;	//Where the tail will point to after this read.
		
		if(next>= UART_RX_BUFFER_SIZE){	//Start from the beginning if all positions of the buffer have been read from.
			next= 0;
		}
		data= buff-> buffer[buff-> tail];	//Read data.
		buff-> tail= next;	//Increment tail.
	}
	
	return data;
}

/*
Returns the number of unread characters in the RX buffer. 
*/
uint16_t uart_available(){
	return rx_buffer.head- rx_buffer.tail;
}

/*
Returns a single char received and stored in the RX buffer. Used in conjunction with 'uart_available()'.
Pops the read character out of the buffer. 
*/
char uart_read(){
	return uart_rx_buffer_pop(&rx_buffer);
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




