/*
 * bmp280.h
 *
 * Created: 04-Nov-18 9:10:11 AM
 *  Author: Ranul Deepanayake
 */ 


#ifndef BMP280_H_
#define BMP280_H_

#include "i2c.h"

#define BMP280_ADDRESS 0x76

//Register definitions.
//Calibration data registers.
#define BMP280_CALIB_00_LSB 0x88	//dig_T1. Unsigned short.
#define BMP280_CALIB_00_MSB 0x89	
#define BMP280_CALIB_01_LSB 0x8A	//dig_T2. Short.
#define BMP280_CALIB_01_MSB 0x8B
#define BMP280_CALIB_02_LSB 0x8C	//dig_T3. Short.
#define BMP280_CALIB_02_MSB 0x8D
#define BMP280_CALIB_03_LSB 0x8E	//dig_P1. Unsigned short.
#define BMP280_CALIB_03_MSB 0x8F
#define BMP280_CALIB_04_LSB 0x90	//dig_P2. Short.
#define BMP280_CALIB_04_MSB 0x91
#define BMP280_CALIB_05_LSB 0x92	//dig_P3. Short.
#define BMP280_CALIB_05_MSB 0x93
#define BMP280_CALIB_06_LSB 0x94	//dig_P4. Short.
#define BMP280_CALIB_06_MSB 0x95
#define BMP280_CALIB_07_LSB 0x96	//dig_P5. Short.
#define BMP280_CALIB_07_MSB 0x97
#define BMP280_CALIB_08_LSB 0x98	//dig_P6. Short.
#define BMP280_CALIB_08_MSB 0x99
#define BMP280_CALIB_09_LSB 0x9A	//dig_P7. Short.
#define BMP280_CALIB_09_MSB 0x9B
#define BMP280_CALIB_10_LSB 0x9C	//dig_P8. Short.
#define BMP280_CALIB_10_MSB 0x9D
#define BMP280_CALIB_11_LSB 0x9E	//dig_P9. Short.
#define BMP280_CALIB_11_MSB 0x9F
#define BMP280_CALIB_12_LSB 0xA0	//Reserved. Should this be read??
#define BMP280_CALIB_12_MSB 0xA1

//Chip ID register.
#define BMP280_CHIP_ID 0XD0

//Chip reset register.
#define BMP280_RESET 0xE0
#define BMP280_RESET_VALUE 0xB6

//Status register.
#define BMP280_STATUS 0xF3

//Measurement control.
#define BMP280_MEASUREMENT_CONTROL 0xF4

//Configuration register.
#define BMP280_CONFIGURATION 0xF5

//Measurement registers.
#define BMP280_PRESSURE_MSB 0xF7
#define BMP280_PRESSURE_LSB 0xF8
#define BMP280_PRESSURE_XLSB 0xF9
#define BMP280_TEMPERATURE_MSB 0xFA
#define BMP280_TEMPERATURE_LSB 0xFB
#define BMP280_TEMPERATURE_XLSB 0xFC

struct bmp280_coefficients{
	uint16_t t1;
	int16_t t2;
	int16_t t3;
};

typedef struct bmp280_coefficients bmp280_coefficient_container;

//Functions.
void bmp280SetDefault();
void bmp280Set(uint8_t configuration, uint8_t control_measurement);
uint32_t bmp280GetTemperature(bmp280_coefficient_container *coefficents);
uint32_t bmp280GetPressure();
uint8_t bmp280GetDeviceId();
void bmp280Reset();
uint8_t bmp280GetDeviceId();
void bmp280GetCalibrationData(bmp280_coefficient_container *coefficents);

#endif /* BMP280_H_ */