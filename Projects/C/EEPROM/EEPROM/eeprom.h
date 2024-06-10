/*
 * eeprom.h
 *
 * Created: 09-Nov-18 8:37:48 PM
 *  Author: ranul
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "i2c.h"
#include "usart328P.h"

uint8_t eepromWrite(uint8_t data);
uint8_t eepromWritePage(uint8_t *data, uint8_t number_of_bytes, uint8_t page_size, uint16_t page_number);
uint8_t eepromReadPage(uint8_t *data, uint8_t page_size, uint16_t page_number);

#endif /* EEPROM_H_ */