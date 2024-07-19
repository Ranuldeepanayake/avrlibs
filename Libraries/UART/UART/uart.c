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

//Includes.
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"

//Buffer for Tx.
typedef struct circular_buffer_tx{
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t count;
	int size;
	volatile char memory[UART_TX_BUFFER_SIZE];
} circular_buffer_tx;

//Buffer for Rx.
typedef struct circular_buffer_rx{
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t count;
	int size;
	volatile char memory[UART_RX_BUFFER_SIZE];
} circular_buffer_rx;


//Initialize the two buffers.
circular_buffer_tx _buffer_tx = { .head = 0, .tail = 0, .count = 0, .size = UART_TX_BUFFER_SIZE };
circular_buffer_rx _buffer_rx = { .head = 0, .tail = 0, .count = 0, .size = UART_RX_BUFFER_SIZE };	

//Error counters.
volatile uint8_t _rx_frame_errors = 0, _rx_overrun_errors = 0, _rx_parity_errors = 0;

typedef uint8_t USART_LED_STATUS;
enum USART_LED_STATUS_OPTIONS {USART_LED_OFF, USART_LED_ON};
	
USART_LED_STATUS usart_led_toggle = USART_LED_OFF;

/*
This ISR is used to pop data from the Tx buffer and send it to the UART data register to be transmitted.
*/
ISR(USART_UDRE_vect){
	usart_led_toggle == USART_LED_ON ? (PORTD |= UART_TX_LED) : (PORTD &= ~UART_TX_LED);		//Turn on RX LED.
	char data = 0x00;
	
	//Pop only if data is in the buffer.
	if(uartAvailable(UART_BUFFER_TYPE_TX)){
		uartBufferPop(UART_BUFFER_TYPE_TX, &data);
		UDR0= data;
	}else{
		UCSR0B &= ~UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;	//Disable the UDRE interrupt to end transmission once the buffer is empty.
	}
	
	usart_led_toggle== USART_LED_ON ? (PORTD &= ~UART_TX_LED) : (PORTD &= ~UART_TX_LED);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
This ISR catches received characters and pushes them into the RX buffer. Error checking is also performed and error counters are incremented appropriately.
*/
ISR(USART_RX_vect){
	usart_led_toggle == USART_LED_ON ? (PORTD |= UART_RX_LED) : (PORTD &= ~UART_RX_LED);		//Turn on RX LED.	
	
	//Read error flags before reading UDR0.
	if(uartFrameError()){
		_rx_frame_errors++;
	}
	if(uartOverrunError()){
		_rx_overrun_errors++;
	}
	if(uartParityError()){
		_rx_parity_errors++;
	}
	
	if(!uartIsFull(UART_BUFFER_TYPE_RX)){
		uartBufferPush(UART_BUFFER_TYPE_RX, UDR0);
	}
	usart_led_toggle== USART_LED_ON ? (PORTD &= ~UART_RX_LED) : (PORTD &= ~UART_RX_LED);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
Initializes the UART peripheral to run on the asynchronous mode.
A baud rate between 9600- 500000 is supported. Frames of 5- 8 data bits are supported.
Parities of none, even and odd are supported. Enables UART and global interrupts.
*/
void uartSet(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits){
	cli();	//Disable global interrupts before setting up the UART peripheral.
	
	//Set the UART registers.
	UCSR0A |= UART_2X_MODE;
	UCSR0B |= (UART_RX_ENABLE | UART_TX_ENABLE); //Enable Tx and Rx.
	UCSR0B |= UART_RX_INTERRUPT_ENABLE; //Enable the necessary interrupts.
	UCSR0C |= (UART_ASYNCHRONOUS_MODE | parity | stop_bits);	//Set mode, parity and stop bits.
	UBRR0L = baud_rate;			//Set the baud rate low byte. 	
	UBRR0H = baud_rate >> 8;	//Set the baud rate high byte.
	
	//Set frame size.
	switch(data_bits){
		case UART_DATA_SIZE_5:
			UCSR0C |= UART_DATA_SIZE_5;
			break;
		case UART_DATA_SIZE_6:
			UCSR0C |= UART_DATA_SIZE_6;
			break;
		case UART_DATA_SIZE_7:
			UCSR0C |= UART_DATA_SIZE_7;
			break;
		case UART_DATA_SIZE_8:
			UCSR0C |= UART_DATA_SIZE_8;
			break;
		//case UART_DATA_SIZE_9:
			//Need to configure two registers.
			//UCSR0B |= 0x04;
		    //UCSR0C |= UART_DATA_SIZE_8;
			//break;
		default:
		    UCSR0C |= UART_DATA_SIZE_8;
	}
	
	sei();	//Re-enable global interrupts.
}

/*
Enables or disables UART activity LEDs. Default LEDs: Rx- PORTD2 and Tx- PORTD3.
*/
void uartSetLed(uint8_t toggle){
	if(toggle == 1){
		usart_led_toggle= USART_LED_ON;
		DDRD |= (UART_TX_LED | UART_RX_LED); //Set LEDs for Tx and Rx.
	}else{
		usart_led_toggle= USART_LED_OFF;
	}
}

/*
Loads a string of characters into the Tx buffer. Carriage return and newline aren't sent.
An ISR will perform the transmission.
*/
uint8_t uartPrint(char *string_pointer){
	//if(strlen(string_pointer) > ((UART_TX_BUFFER_SIZE-1) - uartCount(UART_BUFFER_TYPE_TX))){
		//return 0;
	//}
	
	while((*string_pointer != UART_NULL_CHARACTER) && (!uartIsFull(UART_BUFFER_TYPE_TX))){ //Check for the null character in the string.
		uartBufferPush(UART_BUFFER_TYPE_TX, *(string_pointer++)); //Pushes a character to the buffer.
		//uartSendChar(*(string_pointer++)); //Deprecated.
	}
	
	UCSR0B |= UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;	//Enable the UDRE interrupt to begin transmission through interrupts.
	
	return 1;
}

/*
Loads a string of characters with carriage return and newline into the Tx buffer.
An ISR will perform the transmission.
*/
uint8_t uartPrintLn(char *string_pointer){
	//if(strlen(string_pointer)+2 > ((UART_TX_BUFFER_SIZE-1) - uartCount(UART_BUFFER_TYPE_TX))){
		////return 0;
	//}
	
	uint8_t incrementer = 0;
	
	//Check for the null character in the string and free space in the buffer. Free space is checked as an additional protection against buffer overflow and is not mandatory.
	while((*string_pointer != UART_NULL_CHARACTER) && (!uartIsFull(UART_BUFFER_TYPE_TX))){ 
		uartBufferPush(UART_BUFFER_TYPE_TX, *(string_pointer++));
		//uartSendChar(*(string_pointer++)); //Deprecated.
	}
	//Push the two trailing characters needed for a newline. Free space is checked as an additional protection against buffer overflow and is not mandatory.
	while((incrementer < 1) && (!uartIsFull(UART_BUFFER_TYPE_TX))){ 
		uartBufferPush(UART_BUFFER_TYPE_TX, UART_CARRIAGE_RETURN);
		uartBufferPush(UART_BUFFER_TYPE_TX, UART_NEW_LINE);
		incrementer++;
	}
	
	UCSR0B |= UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;	//Enable the UDRE interrupt to start transmission through interrupts.
	
	return 1;
}

/*
Pushes a received character into the the selected buffer.
Called inside the USART_RX_vect ISR. 
*/
uint8_t uartBufferPush(uint8_t buffer_type, char data){
	if(buffer_type == UART_BUFFER_TYPE_TX){
		//Prevent overwriting when the buffer is full.
		if(_buffer_tx.count ==  _buffer_tx.size){
			return UART_TX_BUFFER_FULL;
		}
		
		_buffer_tx.memory[_buffer_tx.head] = data;
		_buffer_tx.head++;
		_buffer_tx.count++;
		
		//Link the head to the start of the buffer.
		if(_buffer_tx.head == _buffer_tx.size){
			_buffer_tx.head = 0;
		}
		return UART_TX_BUFFER_OK;
		
	}else if(buffer_type == UART_BUFFER_TYPE_RX){
		//Prevent overwriting when the buffer is full.
		if(_buffer_rx.count ==  _buffer_rx.size){
			return UART_RX_BUFFER_FULL;
		}
		
		_buffer_rx.memory[_buffer_rx.head] = data;
		_buffer_rx.head++;
		_buffer_rx.count++;
		
		//Link the head to the start of the buffer.
		if(_buffer_rx.head == _buffer_rx.size){
			_buffer_rx.head = 0;
		}
		return UART_RX_BUFFER_OK;
	}else{
		return UART_UNDEFINED_CONDITION;
	}
}

/*
Pops a received character from the selected buffer.
Called inside the USART_TX_vect ISR.
*/
uint8_t uartBufferPop(uint8_t buffer_type, char *data){
	if(buffer_type == UART_BUFFER_TYPE_TX){
		//Check for an empty buffer.
		if(_buffer_tx.count == 0){
			return UART_TX_BUFFER_EMPTY;
		}
		
		*data = _buffer_tx.memory[_buffer_tx.tail];
		_buffer_tx.tail++;
		_buffer_tx.count--;
		
		//Link the tail to the start of the buffer.
		if(_buffer_tx.tail == _buffer_tx.size){
			_buffer_tx.tail = 0;
		}
		return UART_TX_BUFFER_OK;
		
	}else if(buffer_type == UART_BUFFER_TYPE_RX){
		//Check for an empty buffer.
		if(_buffer_rx.count == 0){
			return UART_RX_BUFFER_EMPTY;
		}
		
		*data = _buffer_rx.memory[_buffer_rx.tail];
		_buffer_rx.tail++;
		_buffer_rx.count--;
		
		//Link the tail to the start of the buffer.
		if(_buffer_rx.tail == _buffer_rx.size){
			_buffer_rx.tail = 0;
		}
		return UART_RX_BUFFER_OK;	
	}else{
		return UART_UNDEFINED_CONDITION;
	}	
}

/*
Returns if unread characters are present in the selected buffer.
*/
uint8_t uartAvailable(uint8_t buffer_type){
	if(buffer_type == UART_BUFFER_TYPE_TX){
		if(_buffer_tx.count > 0){
			return 1;
		}else{
			return 0;
		}
	}else if(buffer_type == UART_BUFFER_TYPE_RX){
		if(_buffer_rx.count > 0){
			return 1;
		}else{
			return 0;
		}
	}else{
		return UART_UNDEFINED_CONDITION;
	}
}

/*
Returns the number of unread characters in the selected buffer.
*/
uint8_t uartCount(uint8_t buffer_type){
	if(buffer_type == UART_BUFFER_TYPE_TX){
		return _buffer_tx.count;
	}else if(buffer_type == UART_BUFFER_TYPE_RX){
		return _buffer_rx.count;
	}else{
		return UART_UNDEFINED_CONDITION;
	}
}

/*
Returns the number of unread characters in the selected buffer.
*/
uint8_t uartIsFull(uint8_t buffer_type){
	if(buffer_type == UART_BUFFER_TYPE_TX){
		if(_buffer_tx.count == UART_TX_BUFFER_SIZE){
			return 1;
		}else{
			return 0;
		}
		
	}else if(buffer_type == UART_BUFFER_TYPE_RX){
		if(_buffer_rx.count == UART_RX_BUFFER_SIZE){
			return 1;
		}else{
			return 0;
		}
	}else{
		return UART_UNDEFINED_CONDITION;
	}
}

/*
Returns the last character in the selected buffer.
*/
uint8_t uartPeek(uint8_t buffer_type, int *data){
	if(buffer_type == UART_BUFFER_TYPE_TX){
		if(uartAvailable(UART_BUFFER_TYPE_TX)){
			*data = _buffer_tx.memory[_buffer_tx.tail];
			return 1;
		}else{
			return 0;
		}
	}else if(buffer_type == UART_BUFFER_TYPE_RX){
		if(uartAvailable(UART_BUFFER_TYPE_RX)){
			*data = _buffer_rx.memory[_buffer_tx.tail];
			return 1;
		}else{
			return 0;
		}
	}else{
		return UART_UNDEFINED_CONDITION;
	}
}

/*
Returns a single char received and stored in the Rx buffer. Used in conjunction with 'uartAvailable()'.
Pops the read character out of the buffer. 
*/
char uartRead(){
	char data = 0x00;
	//Return popped data only if the pop operation is successful.
	if(uartAvailable(UART_BUFFER_TYPE_RX)){
		uartBufferPop(UART_BUFFER_TYPE_RX, &data);
		return data;
	}else{
		return 0x00;
	}
}

/*
Detects a frame error in data reception.
*/
uint8_t uartFrameError(){
	if(UCSR0A & UART_FRAME_ERROR){
		return UART_FRAME_ERROR;
	}else{
		return UART_OK;
	}
}

/*
Returns the frame error count. The value rolls over after 255.
*/
uint8_t uartGetFrameErrorCount(){
	return _rx_frame_errors;
}

/*
Detects a data overrun error in data reception.
*/
uint8_t uartOverrunError(){
	if(UCSR0A & UART_DATA_OVERRUN_ERROR){
		return UART_DATA_OVERRUN_ERROR;
	}else{
		return UART_OK;
	}
}

/*
Returns the overrun error count. The value rolls over after 255.
*/
uint8_t uartGetOverrunErrorCount(){
	return _rx_overrun_errors;
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

/*
Returns the parity error count. The value rolls over after 255.
*/
uint8_t uartGetParityErrorCount(){
	return _rx_parity_errors;
}

/*
(Deprecated) Transmits a single character on the Tx line using polling. Has no relationship with the buffer based Tx mechanism.
*/
void uartSendChar(char data){
	usart_led_toggle == USART_LED_ON ? (PORTD |= UART_TX_LED) : (PORTD &= ~UART_TX_LED);		//Turn on the Tx LED.
	
	while(!(UCSR0A & UART_DATA_REGISTER_EMPTY)); //Check the status of UDRE0. Transmit only when ready.
	UDR0= data;	//Write data to the register.
	//while(!(UCSR0A & (1 << TXC0))); //Wait till the shift register has shifted out all bits.
	
	usart_led_toggle == USART_LED_ON ? (PORTD &= ~UART_TX_LED) : (PORTD &= ~UART_TX_LED);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
(Deprecated) Waits in a loop till a char is received. Has no relationship with the buffer based Rx mechanism.
*/
char uartReadCharWait(){
	//Waits till a character is received in a while loop.
	usart_led_toggle== USART_LED_ON ? (PORTD|= 0x04) : (PORTD&= ~0x04);
	while(!(UCSR0A & (1<< RXC0))); //Check the status of RXC0.
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x04) : (PORTD&= ~0x04);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
	return UDR0;
}




