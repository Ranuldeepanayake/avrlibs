/*
 * header.h
 *
 * Created: 10-Oct-18 8:09:55 PM
 *  Author: Ranul Deepanayake
 */ 


#ifndef HEADER_H_
#define HEADER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h> //Required for dtostrf.
#include <util/delay.h>
#include <stdio.h> //Required for sprintf.

#define F_CPU 16000000
#define BAUDRATE 9600
#define BAUDPRESCALER ((F_CPU/(BAUDRATE* 16UL)) -1)
#define NEWLINE '\n' 
#define CARRIAGE_RETURN '\r'

void setLed();
void setAdc();
void setTimer1();
void writePwm();
void setUart();

void sendChar(unsigned char data);
void sendString(char *string);

float readVoltage();
float map (float value, float inMin, float inMax, float outMin, float outMax);
void debugCode();

#endif /* HEADER_H_ */