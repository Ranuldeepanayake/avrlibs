/*
 * DHT11.c
 *
 * Created: 15-Jan-19 9:24:39 AM
 * Author : Ranul Deepanayake
 * Test bed for the DHT11 library.
 */ 

#include <stdio.h>
#include "usart328P.h"
#include "dht11.h"

void get_humi_int(uint8_t data[]);
void get_temp_int(uint8_t unit, uint8_t data[]);
void get_humi_float(uint8_t data[]);
void get_temp_float(uint8_t unit, uint8_t data[]);
void get_heat_index_float(uint8_t unit, uint8_t data[]);

int main(void)
{	
	setUsart(9600);
	uint8_t data[DHT11_NUM_BYTES];
	
    while (1) 
    {
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		_delay_ms(250);
		if(dht11_measure(data)!= DHT11_ERROR){
			/*get_humi_int(data);
			get_temp_int(DHT11_FARENHEIT, data);
			get_heat_index_float(DHT11_FARENHEIT, data);*/
			print("Humidity: ");
			get_humi_float(data);
			print("\tTemperature: ");
			get_temp_float(DHT11_CELCIUS, data);
			print("\tHeat Index: ");
			get_heat_index_float(DHT11_CELCIUS, data);
			println("");
		}
    }
}

void get_humi_int(uint8_t data[]){
	uint8_t humidity[2];
	char temp[5];
	dht11_get_humidity_integer(data, humidity);
	sprintf(temp, "%d.%d %% \t", humidity[0], humidity[1]);
	print(temp);
}

void get_temp_int(uint8_t unit, uint8_t data[]){
	uint8_t temperature[2];
	char temp[7];	//Important.
	dht11_get_temperature_integer(unit, data, temperature);
	sprintf(temp, "%d.%d \t", temperature[0], temperature[1]);
	print(temp);
}

void get_humi_float(uint8_t data[]){
	char temp[6];
	float humidity= 0;
	humidity= dht11_get_humidity(data);
	dtostrf(humidity, 3, 1, temp);
	print(temp);
}

void get_temp_float(uint8_t unit, uint8_t data[]){
	char temp[6];
	float temperature= 0;
	temperature= dht11_get_temperature(unit, data);
	dtostrf(temperature, 3, 1, temp);
	print(temp);
}

void get_heat_index_float(uint8_t unit, uint8_t data[]){
	char temp[6];
	float heat_index= 0;
	heat_index= dht11_get_heat_index(unit, data);
	dtostrf(heat_index, 3, 1, temp);
	println(temp);
}



