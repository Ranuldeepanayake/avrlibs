/*
 * dht11.h
 *
 * Created: 15-Jan-19 9:25:23 AM
 * Author : Ranul Deepanayake
 * DHT11 sensor library for the ATmega 328P. Works on all GPIO ports and pins.
 * The sensor requires a minimum 2 second delay between polls. 
 * Supports ambient air temperature, relative air humidity and heat index measurement.
 * Supports integer and float results. 
 * Supports error detection through a checksum.
 * Requires 'F_CPU'. 
 */ 


#ifndef DHT11_H_
#define DHT11_H_

//Library includes.
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//Constant attributes and functions.
#ifndef DHT11_DDR_REGISTER 
#define DHT11_DDR_REGISTER DDRD			//Default is PORTD.
#endif
#ifndef DHT11_PORT_REGISTER
#define DHT11_PORT_REGISTER PORTD		//Default is PORTD.
#endif
#ifndef DHT11_PIN_REGISTER
#define DHT11_PIN_REGISTER PIND			//Default is PIND.
#endif
#ifndef DHT11_PIN
#define DHT11_PIN (1<<7) 				//Default is pin 8.
#endif
#define DHT11_PIN_HIGH (DHT11_PORT_REGISTER|= DHT11_PIN);
#define DHT11_PIN_LOW (DHT11_PORT_REGISTER&= ~DHT11_PIN);
#define DHT11_PIN_OUTPUT (DHT11_DDR_REGISTER|= DHT11_PIN);
#define DHT11_PIN_INPUT (DHT11_DDR_REGISTER&= ~DHT11_PIN);
#define DHT11_PIN_INPUT_HIGH (DHT11_PIN_REGISTER & DHT11_PIN)
#define DHT11_PIN_WAIT_MILLISECONDS(DELAY) (_delay_ms(DELAY));
#define DHT11_PIN_WAIT_MICROSECONDS(DELAY) (_delay_us(DELAY));
#define DHT11_WATCHDOG_TIMER_COUNT_DELAY 1
#define DHT11_CELCIUS 10
#define DHT11_FARENHEIT 11
#define DHT11_NUM_BYTES 5
#define DHT11_HUMIDITY_INT_BYTE 0
#define DHT11_HUMIDITY_DEC_BYTE 1
#define DHT11_TEMPERATURE_INT_BYTE 2
#define DHT11_TEMPERATURE_DEC_BYTE 3
#define DHT11_CHECKSUM_BYTE 4
#define DHT11_TIMEOUT_MICROSECONDS 250
#define DHT11_STABILIZE_PULSE_TIME 10
#define DHT11_START_PULSE_TIME 20
#define DHT11_LOW_PULSE_MAX_TIME 30

//Status and error codes.
#define DHT11_OK 0
#define DHT11_ERROR 1

//Functions.
//Request measurement from sensor. Must be called before other functions. 
uint8_t dht11_measure(uint8_t data[]);
//Return relative air humidity as a percentage as a float.
float dht11_get_humidity(uint8_t data[]);
//Return ambient air temperature as a float.
float dht11_get_temperature(uint8_t unit, uint8_t data[]);
//Return relative air humidity as an integer pair.
void dht11_get_humidity_integer(uint8_t data[], uint8_t humidity[]);
//Return ambient air temperature as an integer pair.
void dht11_get_temperature_integer(uint8_t unit, uint8_t data[], uint8_t temperature[]);
//Return heat index as as a float.
float dht11_get_heat_index(uint8_t unit, uint8_t data[]);

/*
Example implementation to read temperature as a float and integer.

//Include the following defines in the the header 'dht11.h'.
#define DHT11_DDR_REGISTER DDRB
#define DHT11_PORT_REGISTER PORTB
#define DHT11_PIN_REGISTER PINB
#define DHT11_PIN (1<<0)	//Pin number 0.

main(){
	
	uint8_t data[DHT11_NUM_BYTES];	//Array to keep the sampled data.
	
	while(1){
		if(dht11_measure(data)!= DHT11_ERROR){
			//Float result.
			float temperature= dht11_get_temperature(DHT11_CELCIUS, data);
			//Do stuff.
			
			//Integer result.
			uint8_t temp[2];	//Array to keep the integer pair. Index 0: integer portion and 1: decimal portion of the sample. 
			dht11_get_temperature_integer(DHT11_CELCIUS, data, temp);
			//Do stuff.
		}
		delay(2000);
	}
}

*/

#endif /* DHT11_H_ */