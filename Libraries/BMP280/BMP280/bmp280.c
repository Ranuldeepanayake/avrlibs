/*
 * bmp280.c
 *
 * Created: 04-Nov-18 8:57:20 AM
 * Author: Ranul Deepanayake
 */ 
#include "bmp280.h"

int32_t bmp280_t_fine= 0;

/*
Set up the sensor with default settings.
*/
void bmp280SetDefault(void){
	bmp280Set(BMP280_MODE_NORMAL, BMP280_OVERSAMPLE_PRESSURE_X16, BMP280_OVERSAMPLE_TEMPERATURE_X2, BMP280_FILTER_2, BMP280_STANDBY_250_MS);
}

/*
Set up the sensor with user specified settings.
*/
void bmp280Set(uint8_t mode, uint8_t oversample_pressure, uint8_t oversample_temperature,  uint8_t iir_filter, uint8_t standby_time){
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_MEASUREMENT_CONTROL_REGISTER);
	i2cWrite(mode | oversample_pressure | oversample_temperature);
	i2cWrite(BMP280_CONFIGURATION_REGISTER);
	i2cWrite(iir_filter | standby_time);
	i2cStop();
}

/*
Saves coefficient data in the host microcontroller.
Must be called once before the first measurement. 
*/
void bmp280GetCalibrationData(bmp280_coefficient_container *coefficents){
	uint16_t temp_u= 0;
	int16_t temp_s= 0;
	
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_CALIB_00_LSB);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	
	//Get temperature coefficient data.
	temp_u= i2cReadByte();
	coefficents->t_1= (i2cReadByte()<< 8) | temp_u;
	
	temp_s= i2cReadByte();
	coefficents->t_2= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->t_3= (i2cReadLastByte()<< 8) | temp_s;
	
	//Get pressure coefficient data.
	temp_u= i2cReadByte();
	coefficents->p_1= (i2cReadByte()<< 8) | temp_u;
	
	temp_s= i2cReadByte();
	coefficents->p_2= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_3= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_4= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_5= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_6= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_7= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_8= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->p_9= (i2cReadLastByte()<< 8) | temp_s;
	
	i2cStop();
}

/*
Get temperature as a float in Celsius with a resolution of two decimal places. Ex- 32.58C.
*/
float bmp280GetTemperature(bmp280_coefficient_container *coefficents){
	int32_t temperature= 0, var_1= 0, var_2= 0;
	uint8_t temp_msb= 0, temp_lsb= 0, temp_xlsb= 0;
	float t= 0;
	
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_TEMPERATURE_MSB);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	temp_msb= i2cReadByte();
	temp_lsb= i2cReadByte();
	temp_xlsb= i2cReadLastByte();
	i2cStop();
	
	//Formula from Adafruit's library.
	temperature= temp_msb;
	temperature= (temperature<< 8) | temp_lsb;
	temperature= (temperature<< 8) | temp_xlsb;  
	temperature= (temperature>> 4);
	//temperature>>= 4; //GIves a bad reading.
	
	var_1= ((((temperature>> 3)- ((int32_t)coefficents->t_1 <<1)))* ((int32_t)coefficents->t_2))>> 11;
	var_2= (((((temperature>> 4)- ((int32_t)coefficents->t_1))* ((temperature>> 4)- ((int32_t)coefficents->t_1)))>> 12)* ((int32_t) coefficents->t_3))>> 14;
	bmp280_t_fine= var_1+ var_2;
	t= (bmp280_t_fine* 5+ 128)>> 8;
	return t/100;
}

/*
Get pressure as a float in Pa with a resolution of two decimal places. Ex- 97588.45Pa.
Implicitly calls the temperature function to  update the 'bmp280_t_fine' global variable.
*/
float bmp280GetPressure(bmp280_coefficient_container *coefficents){
	int32_t pressure= 0; 
	uint8_t pressure_msb= 0, pressure_lsb= 0, presure_xlsb= 0;
	int64_t var_1= 0, var_2= 0, p= 0;
	
	bmp280GetTemperature(coefficents); //Has to be called to update the 'bmp280_t_fine' global variable. 
	
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_PRESSURE_MSB);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	pressure_msb= i2cReadByte();
	pressure_lsb= i2cReadByte();
	presure_xlsb= i2cReadLastByte();
	i2cStop();
	
	//Formula from Adafruit's library.
	pressure= pressure_msb;
	pressure= (pressure<< 8) | pressure_lsb;
	pressure= (pressure<< 8) | presure_xlsb;
	pressure= (pressure>> 4);
	pressure>>= 4;

	var_1 = ((int64_t)bmp280_t_fine) - 128000;
	var_2 = var_1 * var_1 * (int64_t)coefficents-> p_6;
	var_2 = var_2 + ((var_1* (int64_t)coefficents-> p_5)<< 17);
	var_2 = var_2 + (((int64_t)coefficents-> p_4)<< 35);
	var_1 = ((var_1 * var_1 * (int64_t)coefficents-> p_3)>> 8) +
	((var_1 * (int64_t)coefficents-> p_2)<< 12);
	var_1 = (((((int64_t)1)<< 47)+ var_1))* ((int64_t)coefficents-> p_1)>> 33;

	if (var_1 == 0) {
		return 0;  // avoid exception caused by division by zero
	}
	
	p= 1048576- pressure;
	p= (((p<< 31)- var_2)*3125) / var_1;
	var_1= (((int64_t)coefficents-> p_9)* (p>> 13)* (p>> 13))>> 25;
	var_2= (((int64_t)coefficents-> p_8)* p)>> 19;
	p = ((p + var_1 + var_2) >> 8) + (((int64_t)coefficents-> p_7)<< 4);
	
	return (float)p/ 256;
}

/*
Get the device ID (0x58).
*/
uint8_t bmp280GetDeviceId(void){
	uint8_t chip_id= 0;
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_CHIP_ID_REGISTER);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	chip_id= i2cReadLastByte();
	i2cStop();
	return chip_id;
}

/*
Reset the sensor.
*/
void bmp280Reset(void){
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_RESET_REGISTER);
	i2cWrite(BMP280_RESET_VALUE);
	i2cStop();
}

/*
Under development.
*/
void bmp280ForceMeasurement(void){
}

/*
Under development.
*/
uint8_t bmp280GetMeasurementStatus(void){
	return 0;
}

/*
Under development.
*/
uint8_t bmp280GetNvsLoadStatus(void){
	return 0;
}