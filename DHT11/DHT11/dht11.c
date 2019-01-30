/*
 * dht11.c
 *
 * Created: 15-Jan-19 9:25:05 AM
 * Author : Ranul Deepanayake
 */ 

#include "dht11.h"

/*
Initiates communication with the DHT11 sensor and obtains measurements.
Uses a bit banged single wire custom serial communications protocol.
Requires an array of size 'DHT11_NUM_BYTES' to be passed as an argument.
Implements a watchdog counter to prevent system hanging in the event of a sensor communication failure.
Supports error detection through a checksum value sent by the sensor and returns a status code accordingly.
*/
uint8_t dht11_measure(uint8_t data[]){
	uint8_t watchdog_counter= 0;
	
	//Send request signal to the sensor.
	DHT11_PIN_OUTPUT		
	DHT11_PIN_HIGH
	DHT11_PIN_WAIT_MILLISECONDS(DHT11_STABILIZE_PULSE_TIME)	//Stabilize the signal line (not mandatory).
	DHT11_PIN_LOW
	DHT11_PIN_WAIT_MILLISECONDS(DHT11_START_PULSE_TIME)
	DHT11_PIN_HIGH
	
	//Await response signal from the sensor..
	DHT11_PIN_INPUT		//Make pin input.
	while(DHT11_PIN_INPUT_HIGH){
		DHT11_PIN_WAIT_MICROSECONDS(DHT11_WATCHDOG_TIMER_COUNT_DELAY)
		watchdog_counter++;	//Increment the watchdog counter.
		if(watchdog_counter>= DHT11_TIMEOUT_MICROSECONDS){	//Check if the watchdog counter reaches the threshold. Prevents system hanging.
			return DHT11_ERROR;		//Return error code.
		}
	}
	watchdog_counter= 0;	//Reset watchdog counter.
	while(!(DHT11_PIN_INPUT_HIGH)){
		DHT11_PIN_WAIT_MICROSECONDS(DHT11_WATCHDOG_TIMER_COUNT_DELAY)
		watchdog_counter++;
		if(watchdog_counter>= DHT11_TIMEOUT_MICROSECONDS){
			return DHT11_ERROR;
		}
	}
	watchdog_counter= 0;
	while(DHT11_PIN_INPUT_HIGH){
		DHT11_PIN_WAIT_MICROSECONDS(DHT11_WATCHDOG_TIMER_COUNT_DELAY)
		watchdog_counter++;
		if(watchdog_counter>= DHT11_TIMEOUT_MICROSECONDS){
			return DHT11_ERROR;
		}
	}
	
	//Read received data bytes from the sensor.
	for(uint8_t byte= 0; byte< DHT11_NUM_BYTES; byte++){	//For each expected byte.
		for (uint8_t bit= 0; bit< 8; bit++){		//For each expected bit.
			watchdog_counter= 0;
			while(!(DHT11_PIN_INPUT_HIGH)){			//Wait for signal to become high.
				DHT11_PIN_WAIT_MICROSECONDS(DHT11_WATCHDOG_TIMER_COUNT_DELAY)
				watchdog_counter++;
				if(watchdog_counter>= DHT11_TIMEOUT_MICROSECONDS){
					return DHT11_ERROR;
				}
			}
			DHT11_PIN_WAIT_MICROSECONDS(DHT11_LOW_PULSE_MAX_TIME)	//Wait a specified time.
			if(DHT11_PIN_INPUT_HIGH){	 //Consider pulse to be a '1' if input is still high after the delay (read the datasheet for sensor timeings).
				data[byte] = (data[byte]<< 1) | (0x01);		     //Shift left and 'OR' '1' (data is sent MSB first). Shift left first to avoid arithmetic OR.
			}else{				         //Else consider pulse to be a '0'. 
				data[byte] = (data[byte]<< 1);	//Shift left (data is sent MSB first).
			}
			watchdog_counter= 0;
			while(DHT11_PIN_INPUT_HIGH){	//Wait till high pulse is over.
				DHT11_PIN_WAIT_MICROSECONDS(DHT11_WATCHDOG_TIMER_COUNT_DELAY)
				watchdog_counter++;
				if(watchdog_counter>= DHT11_TIMEOUT_MICROSECONDS){
					return DHT11_ERROR;
				}
			}
		}
	}
	
	//Calculate checksum and compare with received checksum to detect corrupt data.
	uint16_t result= 0;
	for(uint8_t i= 0; i< DHT11_NUM_BYTES- 1; i++){
		result+= data[i];	//Add first four bytes together.
	}
	if(result!= data[DHT11_CHECKSUM_BYTE]){	//Compare result with checksum byte and return status code.
		return DHT11_ERROR;
	}else{
		return DHT11_OK;
	}
}

/*
Returns the relative air humidity as a float percentage.
Requires an array of the size 'DHT11_NUM_BYTES' as an argument. 
*/
float dht11_get_humidity(uint8_t data[]){
	float integer= (float) data[DHT11_HUMIDITY_INT_BYTE];
	float decimal= (float) data[DHT11_HUMIDITY_DEC_BYTE];
	return ((integer* 10)+ decimal)/10;
}

/*
Returns the ambient air temperature as a float value according to the specified measurement unit passed as an argument.
Requires an array of the size 'DHT11_NUM_BYTES' as an argument.
*/
float dht11_get_temperature(uint8_t unit, uint8_t data[]){
	float integer= (float) data[DHT11_TEMPERATURE_INT_BYTE];
	float decimal= (float) data[DHT11_TEMPERATURE_DEC_BYTE];
	if(unit== DHT11_CELCIUS){
		return ((integer* 10)+ decimal)/10;
	}else{
		return (((((integer* 10)+ decimal)* 90)/50)+ 320)/ 10;		//Celcius to farenheit conversion. 
	}
}

/*
Stores the relative air humidity as an integer pair in an array..
Requires an array of the size 'DHT11_NUM_BYTES' an argument.
Requires an array of 'size= 2' to store the integer pair. Index: 0 contains the integer portion while index: 1 contains the decimal portion of the sample.
*/
void dht11_get_humidity_integer(uint8_t data[], uint8_t humidity[]){
	humidity[0]= data[DHT11_HUMIDITY_INT_BYTE];
	humidity[1]= data[DHT11_HUMIDITY_DEC_BYTE];
}

/*
Stores the ambient air temperature as an integer pair in an array according to the specified measurement unit passed as an argument.
Requires an array of the size 'DHT11_NUM_BYTES' an argument.
Requires an array of 'size= 2' to store the integer pair. Index: 0 contains the integer portion while index: 1 contains the decimal portion of the sample.
*/
void dht11_get_temperature_integer(uint8_t unit, uint8_t data[], uint8_t temperature[]){
	if(unit== DHT11_CELCIUS){
		temperature[0]= data[DHT11_TEMPERATURE_INT_BYTE];
		temperature[1]= data[DHT11_TEMPERATURE_DEC_BYTE];
	}
	else{
		temperature[0]= (((float) data[DHT11_TEMPERATURE_INT_BYTE])* 1.8)+ 32;
		temperature[1]= (((float) data[DHT11_TEMPERATURE_DEC_BYTE])* 1.8)+ 32;
	}
}

/*
Returns the heat index as a float value according to the specified measurement unit passed as an argument.
Requires an array of the size 'DHT11_NUM_BYTES' as an argument.
*/
float dht11_get_heat_index(uint8_t unit, uint8_t data[]){
	float temperature= dht11_get_temperature(DHT11_FARENHEIT, data);
	float humidity= dht11_get_humidity(data);
	float heat_index= 0;
	
	//Algorithm to find the heat index (Steadman & Rothfusz).
	heat_index = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) + (humidity * 0.094));
	
	if (heat_index > 79) {
		heat_index = -42.379 +
		2.04901523 * temperature +
		10.14333127 * humidity +
		-0.22475541 * temperature*humidity +
		-0.00683783 * pow(temperature, 2) +
		-0.05481717 * pow(humidity, 2) +
		0.00122874 * pow(temperature, 2) * humidity +
		0.00085282 * temperature*pow(humidity, 2) +
		-0.00000199 * pow(temperature, 2) * pow(humidity, 2);
		
		//Adjustment.
		if((humidity < 13) && (temperature >= 80.0) && (temperature <= 112.0))
		heat_index -= ((13.0 - humidity) * 0.25) * sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);
		
		//Adjustment.
		else if((humidity > 85.0) && (temperature >= 80.0) && (temperature <= 87.0))
		heat_index += ((humidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
	}
	
	//Convert C to F if specified.
	if(unit== DHT11_CELCIUS){
		heat_index= (heat_index- 32)* 0.55555;	
	}
	
	return heat_index;
}

