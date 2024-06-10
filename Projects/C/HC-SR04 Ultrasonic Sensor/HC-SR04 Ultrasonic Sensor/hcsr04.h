/*
 * hcsr04.h
 *
 * Created: 22-Nov-18 8:47:57 PM
 *  Author: ranul
 */ 


#ifndef HCSR04_H_
#define HCSR04_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart328P.h"

void hcsr04SetTrigger();
void hcsr04ReadEcho();
uint16_t getMicroseconds();
uint8_t getState();

#endif /* HCSR04_H_ */