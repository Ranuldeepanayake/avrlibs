/*
 * header.h
 *
 * Created: 02-Oct-18 10:29:28 AM
 *  Author: Ranul Deepanayake
 */ 


#ifndef HEADER_H_
#define HEADER_H_

#include <avr/io.h>
#include <util/delay.h>

typedef uint8_t bool;
enum bool{false, true};

void testLed();
void setDigitalInputs();
void setDigitalInputsSamePort();

uint8_t readPins();
uint16_t readPins16Bit();

void buttonStatus(uint8_t input);
void buttonStatusInverted(char input);
void buttonStatusSamePort(uint8_t input);

#endif /* HEADER_H_ */