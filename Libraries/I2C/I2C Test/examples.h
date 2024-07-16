/*
 * examples.h
 *
 * Created: 06-Nov-18 9:22:08 AM
 * Author: Ranul Deepanayake
 * Demonstrates I2C communication with the ATmega328P as a master and the BOSCH BMP280 and 24LC512 EEPROM as slaves.
 */ 


#ifndef EXAMPLES_H_
#define EXAMPLES_H_

#include "i2c.h"
#include <stdio.h>
#include <util/delay.h>

uint8_t bmp280Simple();

#endif /* EXAMPLES_H_ */