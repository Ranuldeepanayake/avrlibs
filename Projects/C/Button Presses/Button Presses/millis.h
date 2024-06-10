/*
 * millis.h
 *
 * Created: 29-Oct-18 1:08:16 PM
 *  Author: ranul
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t millis();
void setTimer();

#endif /* MILLIS_H_ */