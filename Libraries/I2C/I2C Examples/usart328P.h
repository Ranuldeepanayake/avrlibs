/*
 * usart328P.h
 *
 * Created: 30-Oct-18 6:54:33 PM
 * Author: Ranul Deepanayake.
 * USART library for ATmega328P. Includes RX and TX LEDs and selectable baud rate.
 */ 


#ifndef USART328P_H_
#define USART328P_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void setUsart(uint16_t baud_rate);	//Set USART functionality. A baud rate between 9600 and 57600 is supported. Definition of 'F_CPU' is necessary.
void setUsartLed(int toggle);	//Enables or disables USART LEDs. LEDs: RX- PORTD2 and TX- PORTD3.
void sendChar(char data);	//Sends a single character.
void print(char *string_pointer);	//Sends a string. Carriage return and newline aren't sent.
void println(char *string_pointer);	//Sends a string. Carriage return and newline are appended to the string.
char readCharWait();	//Return a received character. Waits till a character is received in a while loop. 
char readChar();	//**Haven't completed. Return a character set by the receive interrupt.

#endif /* USART328P_H_ */