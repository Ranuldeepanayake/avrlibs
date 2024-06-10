/*
 * CFile1.c
 *
 * Created: 04-Nov-18 8:57:20 AM
 * Author: Ranul Deepanayake
 */ 
#include "bmp280.h"

void bmp280SetDefault(){
	/*i2cDelayedStart(BMP280_ADDRESS, WRITE);
	i2cWrite(BMP280_CONFIGURATION);
	i2cWrite(configuration);
	i2cWrite(BMP280_MEASUREMENT_CONTROL);
	i2cWrite(control_measurement);
	i2cStop();*/
}

void bmp280Set(uint8_t configuration, uint8_t control_measurement){
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_CONFIGURATION);
	i2cWrite(configuration);
	i2cWrite(BMP280_MEASUREMENT_CONTROL);
	i2cWrite(control_measurement);
	i2cStop();
}

uint32_t bmp280GetTemperature(bmp280_coefficient_container *coefficents){
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_TEMPERATURE_MSB);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	uint8_t temp_msb= i2cReadByte();
	uint8_t temp_lsb= i2cReadByte();
	uint8_t temp_xlsb= i2cReadLastByte();
	stop();
	
	int32_t temperature= temp_msb; //Datasheet says unsigned integer?
	temperature= (temperature<< 8) | temp_lsb;
	temperature= (temperature<< 8) | temp_xlsb;  
	temperature= (temperature>> 4);
	
	int32_t var1= 0, var2= 0, t_fine= 0, T= 0;
	var1= ((((temperature>> 3)- ((int32_t)coefficents->t1 <<1)))* ((int32_t)coefficents->t2))>> 11;
	var2= (((((temperature>> 4)- ((int32_t)coefficents->t1))* ((temperature>> 4)- ((int32_t)coefficents->t1)))>> 12)* ((int32_t) coefficents->t3))>> 14;
	t_fine= var1+ var2;
	T= (t_fine* 5+ 128)>> 8;
	return T;
}

uint32_t bmp280GetPressure(){
	return 0;
}

uint8_t bmp280GetStatus(){
	return 0;
}

void bmp280Reset(){
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_RESET);
	i2cWrite(BMP280_RESET_VALUE);
	stop();
}

uint8_t bmp280GetDeviceId(){
	int chip_id= 0;
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_CHIP_ID);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	chip_id= i2cReadLastByte();
	stop();
	return chip_id;
}

void bmp280GetCalibrationData(bmp280_coefficient_container *coefficents){
	i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
	i2cWrite(BMP280_CALIB_00_LSB);
	i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
	
	uint16_t temp_u= 0;
	int16_t temp_s= 0;
	
	temp_u= i2cReadByte();
	coefficents->t1= (i2cReadByte()<< 8) | temp_u;
	
	temp_s= i2cReadByte();
	coefficents->t2= (i2cReadByte()<< 8) | temp_s;
	
	temp_s= i2cReadByte();
	coefficents->t3= (i2cReadLastByte()<< 8) | temp_s;
	
	i2cStop();
}

