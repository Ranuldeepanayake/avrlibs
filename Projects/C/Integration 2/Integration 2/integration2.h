/*
 * integration2.h
 *
 * Created: 24-Dec-18 1:18:50 PM
 * Author: Ranul Deepanayake
 * An integration project which reads two analog devices, uses two circular buffers, a pushbutton and an LED display.
 */ 


#ifndef INTEGRATION2_H_
#define INTEGRATION2_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"
#include "ssd1306.h"
#include "stringfunctions.h"

void setADC();

#endif /* INTEGRATION2_H_ */