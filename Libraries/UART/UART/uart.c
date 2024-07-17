/*
 * Usart328P.c
 *
 * Created: 30-Oct-18 6:53:44 PM
 * Author : Ranul Deepanayake
 */ 
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

typedef uint8_t USART_LED_STATUS;
enum USART_LED_STATUS_OPTIONS {USART_LED_OFF, USART_LED_ON};
	
USART_LED_STATUS usart_led_toggle = USART_LED_OFF;

ISR(USART_UDRE_vect){
	usart_led_toggle == USART_LED_ON ? (PORTD |= UART_TX_LED) : (PORTD &= ~UART_TX_LED);		//Turn on RX LED.
	char data = 0x00;
	
	if(uartAvailable(UART_BUFFER_TYPE_TX)){
		uartBufferPop(UART_BUFFER_TYPE_TX, &data);
		UDR0= data;
	}else{
		UCSR0B &= ~UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;	//Disable the UDRE interrupt to end transmission once the buffer is empty.
	}
	
	usart_led_toggle== USART_LED_ON ? (PORTD &= ~UART_TX_LED) : (PORTD &= ~UART_TX_LED);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

//The ISR catches received characters and pushes them into the RX buffer.
ISR(USART_RX_vect){
	usart_led_toggle == USART_LED_ON ? (PORTD |= UART_RX_LED) : (PORTD &= ~UART_RX_LED);		//Turn on RX LED.	
	uartBufferPush(UART_BUFFER_TYPE_TX, UDR0);
	usart_led_toggle== USART_LED_ON ? (PORTD &= ~UART_RX_LED) : (PORTD &= ~UART_RX_LED);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
Initializes the UART peripheral. 
A baud rate between 9600- 57600 is supported.
Frames of 5- 8 data bits are supported.
*/
void uartSet(uint16_t baud_rate, uint8_t data_bits, uint8_t parity, uint8_t stop_bits){
	cli();	//Disable global interrupts before setting up the UART peripheral.
	
	//Set the UART registers.
	UBRR0H = baud_rate >> 8;	//Set the baud rate high byte.
	UBRR0L = baud_rate;	//Set the baud rate low byte.
	UCSR0B = (UART_RX_ENABLE | UART_TX_ENABLE); //Enable Tx and Rx.
	UCSR0B |= (UART_RX_INTERRUPT_ENABLE); //Enable the necessary interrupts. 		
	
	//Set frame size.
	/*switch(data_bits){
		case 5:
			UCSR0C = UART_DATA_SIZE_5
			break;
		case 6:
			UCSR0C = UART_DATA_SIZE_6
			break;
		case 7:
			UCSR0C = UART_DATA_SIZE_7
			break;
		case 8:
			UCSR0C = UART_DATA_SIZE_8
			break;
		//case 9:
		   //UCSR0C |= UART_DATA_SIZE_9
		//break;
	}*/
	
	//***Double check the below.
	UCSR0C |= (UART_ASYNCHRONOUS_MODE | parity | stop_bits);	//Set mode, parity and stop bits.
	UCSR0C |= UART_DATA_SIZE_8
	UCSR0A &= (0<< U2X0);	//Set this bit to 0 to disable 2x mode.
	
	
	sei();	//Re-enable global interrupts.
}

/*
Enables or disables UART activity LEDs. Default LEDs: RX- PORTD2 and TX- PORTD3.
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
(Deprecated) Transmits a single character on the Tx line using polling.
*/
void uartSendChar(char data){
	usart_led_toggle == USART_LED_ON ? (PORTD |= UART_TX_LED) : (PORTD &= ~UART_TX_LED);		//Turn on the Tx LED.
	
	while(!(UCSR0A & UART_DATA_REGISTER_EMPTY)); //Check the status of UDRE0. Transmit only when ready.
	UDR0= data;	//Write data to the register.
	//while(!(UCSR0A & (1 << TXC0))); //Wait till the shift register has shifted out all bits.
	
	usart_led_toggle == USART_LED_ON ? (PORTD &= ~UART_TX_LED) : (PORTD &= ~UART_TX_LED);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

/*
Transmits a string of characters on the TX line. Carriage return and newline aren't sent.
*/
void uartPrint(char *string_pointer){
	while((*string_pointer != UART_NULL_CHARACTER) && (!uartIsFull(UART_BUFFER_TYPE_TX))){ //Check for the null character in the string.
		uartBufferPush(UART_BUFFER_TYPE_TX, *(string_pointer++)); //Pushes a character to the buffer.
		//uartSendChar(*(string_pointer++)); //Deprecated.
	}
	
	UCSR0B |= UART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE;	//Enable the UDRE interrupt to begin transmission through interrupts.
}

/*
Transmits a string of characters on the TX line. Carriage return and newline are sent.
*/
void uartPrintLn(char *string_pointer){
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
}

/*
Pushes a received character into the RX buffer.
Called inside the USART_RX ISR. 
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
Pops a received character from the RX buffer.
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
Returns if unread characters are present in the buffers.
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
Returns the number of unread characters in the buffers.
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
Returns the number of unread characters in the buffers.
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
Returns the last character in the buffers.
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
Returns a single char received and stored in the RX buffer. Used in conjunction with 'uart_available()'.
Pops the read character out of the buffer. 
*/
char uartRead(){
	char data = 0x00;
	//Return popped data only if the pop operation is successful.
	if(uartBufferPop(UART_BUFFER_TYPE_RX, &data) == UART_RX_BUFFER_OK){
		return data;
	}else{
		return 0x00;
	}
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




