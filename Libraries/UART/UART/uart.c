/*
 * Usart328P.c
 *
 * Created: 30-Oct-18 6:53:44 PM
 * Author : Ranul Deepanayake
 */ 
#include "uart.h"

struct circular_buffer rx_buffer;	//The RX buffer.

typedef uint8_t USART_LED_STATUS;
enum USART_LED_STATUS_OPTIONS {USART_LED_OFF, USART_LED_ON};
	
USART_LED_STATUS usart_led_toggle = USART_LED_OFF;

//The ISR catches received characters and pushes them into the RX buffer.
ISR(USART_RX_vect){
	usart_led_toggle == USART_LED_ON ? (PORTD|= 0x04) : (PORTD&= ~0x04);		//Turn on RX LED.	
	uartRxBufferPush(&rx_buffer, UDR0);
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x04) : (PORTD&= ~0x04);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
Initializes the UART peripheral. 
A baud rate between 9600- 57600 is supported.
Frames of 5- 8 data bits are supported.
*/
void uartSet(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits){
	//Set the USART registers.
	UBRR0H = baud_rate >> 8;	//Set the baud rate high byte.
	UBRR0L = baud_rate;	//Set the baud rate low byte.
	UCSR0B = (UART_RX_ENABLE | UART_TX_ENABLE); //Enable Tx and Rx.
	UCSR0B |= (UART_RX_INTERRUPT_ENABLE); //Enable the Rx interrupt, 		
	
	//Double check the below.
	UCSR0C = (UART_ASYNCHRONOUS_MODE | parity | stop_bits);	//Set mode, parity and stop bits.
	UCSR0C|= ((1<< UCSZ01) | (1<< UCSZ00));	//Set character size (8 bits).
	UCSR0A &= (0<< U2X0);	//Set this bit to 0 to disable 2x mode.
	
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
	
	//Set up the Rx buffer.
	rx_buffer.head= 0;
	rx_buffer.tail= 0;
	
	sei();	//Set global interrupts for the Rx ISR.
}

/*
Enables or disables UART activity LEDs. LEDs: RX- PORTD2 and TX- PORTD3.
*/
void uartSetLed(int toggle){
	if(toggle== 1){
		usart_led_toggle= USART_LED_ON;
		DDRD|= 0x0C; //Set LEDs for TX and RX.
	}else{
		usart_led_toggle= USART_LED_OFF;
	}
}

/*
Transmits a single character on the Tx line.
*/
void uartSendChar(uint8_t data){
	usart_led_toggle== USART_LED_ON ? (PORTD|= 0x08) : (PORTD&= ~0x08);		//Turn on the Tx LED.
	while(!(UCSR0A & UART_DATA_REGISTER_EMPTY)); //Check the status of UDRE0
	UDR0= data;	//Write data to the register.
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x08) : (PORTD&= ~0x08);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
Transmits a string of characters on the TX line. Carriage return and newline aren't sent.
*/
void uartPrint(char *string_pointer){
	while(*string_pointer!= UART_NULL_CHARACTER){ //Check for the null character in the string.
		uartSendChar((uint8_t) *(string_pointer++)); //Sends the value in the memory location.
	}
}

/*
Transmits a string of characters on the TX line. Carriage return and newline are sent.
*/
void uartPrintLn(char *string_pointer){
	while(*string_pointer!= UART_NULL_CHARACTER){ //Check for the null character in the string.
		uartSendChar((uint8_t) *(string_pointer++)); //Sends the value in the memory location.
	}
	uartSendChar(UART_CARRIAGE_RETURN);	//Print carriage return.
	uartSendChar(UART_NEW_LINE); //Print newline.
}

/*
Pushes a received character into the RX buffer.
Called inside the USART_RX ISR. 
*/
void uartRxBufferPush(struct circular_buffer *buff, uint8_t data){
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
char uartRxBufferPop(struct circular_buffer *buff){
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
uint16_t uartAvailable(){
	return rx_buffer.head- rx_buffer.tail;
}

/*
Returns a single char received and stored in the RX buffer. Used in conjunction with 'uart_available()'.
Pops the read character out of the buffer. 
*/
char uartRead(){
	return uartRxBufferPop(&rx_buffer);
}

/*
Waits in a loop till a char is received. Has no relationship with the buffer based Rx mechanism.
*/
char uartReadCharWait(){
	//Waits till a character is received in a while loop.
	usart_led_toggle== USART_LED_ON ? (PORTD|= 0x04) : (PORTD&= ~0x04);
	while(!(UCSR0A & (1<< RXC0))); //Check the status of RXC0.
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x04) : (PORTD&= ~0x04);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
	return UDR0;
}

/*
Detects a frame error in data reception.
*/
uint8_t uartFrameEror(){
	if(UCSR0A & UART_FRAME_ERROR){
		return UART_FRAME_ERROR;
	}else{
		return UART_OK;
	}
}

/*
Detects a data overrun error in data reception.
*/
uint8_t uartDataOverrunError(){
	if(UCSR0A & UART_DATA_OVERRUN_ERROR){
		return UART_DATA_OVERRUN_ERROR;
	}else{
		return UART_OK;
	}
}

/*
Detects a parity error in data reception.
*/
uint8_t uartParityError(){
	if(UCSR0A & UART_PARITY_ERROR){
		return UART_PARITY_ERROR;
	}else{
		return UART_OK;
	}
}




