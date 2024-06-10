/*
 * Millis 328P.c
 *
 * Created: 29-Oct-18 8:53:17 PM
 * Author : Ranul Deepanayake
 * A static library which implements the millis function using Timer0 for the Microchip ATmega328P.
 * This does not break Timer0 functions such as '_delay_ms()'.
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void setTimer();
uint16_t millis();

#endif /* MILLIS_H_ */