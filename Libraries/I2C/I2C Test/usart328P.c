/*
 * Usart328P.c
 *
 * Created: 30-Oct-18 6:53:44 PM
 * Author : Ranul Deepanayake
 * USART library for ATmega328P. Includes RX and TX LEDs and selectable baud rate.
 */ 
#include "usart328P.h"

typedef uint8_t USART_LED_STATUS;
enum USART_LED_STATUS_OPTIONS{USART_LED_OFF, USART_LED_ON};

unsigned long baud;	//Used for calculating the baud rate. 9600- 57600 is supported.
USART_LED_STATUS usart_led_toggle= USART_LED_OFF;
volatile char receive_char= 0;

ISR(USART_RX_vect){
	usart_led_toggle== USART_LED_ON ? (PORTD|= 0x04) : (PORTD&= ~0x04);		//Turn on RX LED.
	receive_char= (char)UDR0;
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x04) : (PORTD&= ~0x04);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

void setUsart(uint16_t baud_rate){
	//Set the USART registers.
	baud= (((F_CPU)/(baud_rate* (unsigned long)16))- 1);	//Calculate value for the baud registers.
	UBRR0H= (uint8_t) baud>> 8;	//Set the baud high byte.
	UBRR0L= (uint8_t)  baud;	//Set the baud low byte.
	UCSR0A&= (0<< U2X0);	//Set this bit to 0 to disable 2x mode.
	UCSR0B|= ((1<< RXEN0) | (1<< TXEN0));	//Enable TX and RX.
	UCSR0C|= ((1<< UCSZ01) | (1<< UCSZ00));	//Set character size (8 bits).
	
	UCSR0B|= (1<< RXCIE0); //Enable RXCIE0 interrupt. Not necessary.
	sei();
}

void setUsartLed(int toggle){
	//Enables or disables USART LEDs. LEDs: RX- PORTD2 and TX- PORTD3.
	if(toggle== 1){
		usart_led_toggle= USART_LED_ON;
		DDRD|= 0x0C; //Set LEDs for TX and RX.
		}else{
		usart_led_toggle= USART_LED_OFF;
	}
}

void sendChar(char data){
	//Sends a single character.
	usart_led_toggle== USART_LED_ON ? (PORTD|= 0x08) : (PORTD&= ~0x08);		//Turn on TX LED.
	while(!(UCSR0A & (1<< UDRE0))); //Check the status of UDRE0
	UDR0= data;	//Write data to the register.
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x08) : (PORTD&= ~0x08);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
}

void print(char *string_pointer){
	//Sends a string. Carriage return and newline aren't sent.
	while(*string_pointer!= 0x00){ //Check for the null character in the string.
		sendChar(*string_pointer); //Sends the value in the memory location.
		//sendCharPointer(stringPointer); //Sends the memory location.
		string_pointer++; //Increment the pointer.
	}
}

void println(char *string_pointer){
	//Sends a string. Carriage return and newline are appended to the string.
	while(*string_pointer!= 0x00){ //Check for the null character in the string.
		sendChar(*string_pointer); //Sends the value in the memory location.
		//sendCharPointer(stringPointer); //Sends the memory location.
		string_pointer++; //Increment the pointer.
	}
	sendChar('\r');	//Print carriage return.
	sendChar('\n'); //Print newline.
}

char readCharWait(){
	//Waits till a character is received in a while loop.
	usart_led_toggle== USART_LED_ON ? (PORTD|= 0x04) : (PORTD&= ~0x04);
	while(!(UCSR0A & (1<< RXC0))); //Check the status of RXC0.
	usart_led_toggle== USART_LED_ON ? (PORTD&= ~0x04) : (PORTD&= ~0x04);	//Turn off TX LED. Used for resiliency against turning off the LEDs within interrupts.
	return UDR0;
}

char readChar(){
	//Return the character set by the receive interrupt.
	return receive_char;
}

