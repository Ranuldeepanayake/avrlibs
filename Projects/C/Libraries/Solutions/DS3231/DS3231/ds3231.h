/*
 * ds3231.h
 *
 * Created: 29-Dec-18 4:01:51 PM
 *  Author: ranul
 */ 


#ifndef DS3231_H_
#define DS3231_H_

#include <avr/io.h>
#include <string.h>
#include "i2c.h"
#include "stringfunctions.h"

#define DS3231_SLAVE_ADDRESS 0x68
#define DS3231_SECOND_ADDRESS 0x00
#define DS3231_MINUTE_ADDRESS 0x01
#define DS3231_HOUR_ADDRESS 0x02
#define DS3231_DAY_ADDRESS 0x03
#define DS3231_DATE_ADDRESS 0x04

#define DS3231_12HOUR_SET 0x40

#define DS3231_CLEAR 0
#define DS3231_SET 1

uint8_t ds3231_get_date_time(char array[]);
uint8_t ds3231_get_second();
uint8_t ds3231_get_minute();
uint8_t ds3231_get_hour();

void ds3231_get_time_all(char array[], uint8_t array_size);
void ds3231_get_date_all(char array[], uint8_t array_size);

#endif /* DS3231_H_ */