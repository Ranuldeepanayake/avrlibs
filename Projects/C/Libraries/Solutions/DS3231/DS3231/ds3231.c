/*
 * ds3231.c
 *
 * Created: 29-Dec-18 4:01:31 PM
 * Author: Ranul Deepanayake
 * Add support for the 12hr format.******************************************************
 */ 

#include "ds3231.h"

uint8_t ds3231_get_date_time(char array[]){
	char temp_string[5];
	uint8_t data[7];
	
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(0x00);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	for(uint8_t byte= 0; byte< 7; byte++){
		data[byte]= i2cReadByte();
		if(byte== 6){
			data[byte]= i2cReadLastByte();
		}
	}
	i2cStop();
	
	data[0]= ((data[0]>> 4)* 10) + (data[0] & 0x0F);	//Get seconds in tens and ones separately and add them.
	data[1]= ((data[1]>> 4)* 10) + (data[1] & 0x0F);
	data[2]= (((data[2] & 0x10)>> 4)* 10)+ (data[2] & 0x0F);
	data[3]= 0;
	data[4]= ((data[4]>> 4)* 10) + (data[4] & 0x0F);
	data[5]= (((data[5] & 0x10)>> 4)* 10)+ (data[5] & 0x0F);
	data[6]= ((data[6]>> 4)* 10) + (data[6] & 0x0F);
	
	for(uint8_t i= 0; i< 7; i++){
		memset(temp_string, 0x00, 5);
		sprintf2(data[i], temp_string);
		strcat(array, temp_string);
	}
	return 0;
}

void ds3231_get_time_all(char array[], uint8_t array_size){
	char temp_string[3];
	uint8_t data[3];
	uint8_t time_format_12h= DS3231_CLEAR;
	
	//Clear the array parameter each time the function is called.
	memset(array, 0x00, array_size);						
	
	//Check for the 12hr/24hr format flag.
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(DS3231_HOUR_ADDRESS);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	data[0]= i2cReadLastByte();
	i2cStop();
	if(data[0] & DS3231_12HOUR_SET){
		time_format_12h= DS3231_SET;
	}
	
	//Request for time data.
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(DS3231_SECOND_ADDRESS);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	for(uint8_t byte= 0; byte< 3; byte++){
		if(byte== 2){
			data[byte]= i2cReadLastByte();
		}
		else{
			data[byte]= i2cReadByte();
		}
	}
	i2cStop();
	
	data[0]= ((data[0]>> 4)* 10) + (data[0] & 0x0F);	//Get seconds in tens and ones separately and add them.
	data[1]= ((data[1]>> 4)* 10) + (data[1] & 0x0F);
	//Handle 12hr and 24hr formats.
	if(time_format_12h){
		
	}else{
		data[2]= (((data[2] & 0x30)>> 4)* 10)+ (data[2] & 0x0F);
	}

	for(uint8_t i= 0; i< 3; i++){
		//memset(temp_string, 0x00, 5);
		sprintf2(data[2- i], temp_string);
		strcat(array, temp_string);
		if(i!= 2){
			strcat(array, ":");
		}
	}
}

void ds3231_get_date_all(char array[], uint8_t array_size){
	char temp_string[5];
	uint8_t data[3];
	memset(array, 0x00, array_size);						//Clear the array parameter each time the function is called.
	
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(DS3231_DATE_ADDRESS);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	for(uint8_t byte= 0; byte< 3; byte++){
		if(byte== 2){
			data[byte]= i2cReadLastByte();
		}
		else{
			data[byte]= i2cReadByte();
		}
	}
	i2cStop();
	
	data[0]= ((data[0]>> 4)* 10) + (data[0] & 0x0F);
	data[1]= (((data[1] & 0x10)>> 4)* 10)+ (data[1] & 0x0F);
	data[2]= ((data[2]>> 4)* 10) + (data[2] & 0x0F);
	
	for(uint8_t i= 0; i< 3; i++){
		//memset(temp_string, 0x00, 5);
		sprintf2(data[i], temp_string);
		strcat(array, temp_string);
		if(i!= 2){
			strcat(array, "-");
		}
	}
}

uint8_t ds3231_get_second(){
	uint8_t second= 0;
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(DS3231_SECOND_ADDRESS);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	second= i2cReadLastByte();
	i2cStop();
	second= ((second>> 4)* 10) + (second & 0x0F);	//Get seconds in tens and ones separately and add them.
	return second;
}

uint8_t ds3231_get_minute(){
	uint8_t minute= 0;
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(DS3231_MINUTE_ADDRESS);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	minute= i2cReadLastByte();
	i2cStop();
	minute= ((minute>> 4)* 10) + (minute & 0x0F);
	return minute;
}

uint8_t ds3231_get_hour(){
	uint8_t hour= 0;
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_WRITE);
	i2cWrite(DS3231_HOUR_ADDRESS);
	i2cDelayedStart(DS3231_SLAVE_ADDRESS, I2C_READ);
	hour= i2cReadLastByte();
	i2cStop();
	hour= (((hour & 0x10)>> 4)* 10)+ (hour & 0x0F);
	return hour;
}

uint8_t ds3231_get_hour_format(){
	
}

uint8_t ds3231_get_am_pm(){
	
}

uint8_t ds3231_get_day(){
	return 0;
}

uint8_t ds3231_get_date(){
	return 0;
}

uint8_t ds3231_get_month(){
	return 0;
}

uint8_t ds3231_get_year(){
	return 0;
}

uint8_t ds3231_get_century(){
	return 0;
}

uint8_t ds3231_set_second(){
	return 0;
}

uint8_t ds3231_set_minute(){
	return 0;
}

uint8_t ds3231_set_hour(){
	return 0;
}

uint8_t ds3231_set_day(){
	return 0;
}

uint8_t ds3231_set_date(){
	return 0;
}

uint8_t ds3231_set_month(){
	return 0;
}

uint8_t ds3231_set_year(){
	return 0;
}

uint8_t ds3231_set_century(){
	return 0;
}

void ds3231_set_12h_format(){
	
}

uint8_t ds3231_get_temperature(){
	return 0;
}

uint8_t ds3231_oscillator_enable(){
	return 0;
}

uint8_t ds3231_battery_backed_sqw_enable(){
	return 0;
}

uint8_t ds3231_execute_tcxo_algorithm(){
	return 0;
}

uint8_t ds3231_sqw_rate_select(){
	return 0;
}

uint8_t ds3231_sqw_interrupt_control(){
	return 0;
}

uint8_t ds3231_alarm_1_interrupt_enable(){
	return 0;
}

uint8_t ds3231_alarm_2_interrupt_enable(){
	return 0;
}

uint8_t ds3231_oscillator_stopped(){
	return 0;
}

uint8_t ds3231_clock32khz_output_enable(){
	return 0;
}

uint8_t ds3231_busy(){
	return 0;
}

uint8_t ds3231_alarm_1_triggered(){
	return 0;
}

uint8_t ds3231_alarm_2_triggered(){
	return 0;
}
