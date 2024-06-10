/*
 * main.h
 *
 * Created: 29-Oct-18 12:38:28 PM
 * Author: Ranul Deepanayake
 * Counts the number of button presses which occurred within a time duration and outputs the count through an LED.
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "millis.h"

void setPins();
uint8_t readPortB();
void countPresses(int port_b_result);
void blinkLed(int presses);

#endif /* MAIN_H_ */