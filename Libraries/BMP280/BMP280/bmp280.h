/*
 * bmp280.h
 *
 * Created: 04-Nov-18 9:10:11 AM
 * Author: Ranul Deepanayake
 * BOSCH BMP280 library for the ATmega328P using I2C.
 * Supports selectable sensor configuration options.
 * Returns temperature and pressure as floats.
 * The pressure function implicitly calls the temperature function to  update the 'bmp280_t_fine' global variable.
 * Saves coefficient data in the host microcontroller.
 */ 

#ifndef BMP280_H_
#define BMP280_H_

#include "i2c.h"

//Defines.
#define BMP280_ADDRESS 0x76

//Configuration registers.
#define BMP280_STATUS_REGISTER 0xF3
#define BMP280_MEASUREMENT_CONTROL_REGISTER 0xF4
#define BMP280_CONFIGURATION_REGISTER 0xF5
#define BMP280_RESET_REGISTER 0xE0
#define BMP280_CHIP_ID_REGISTER 0XD0

#define BMP280_RESET_VALUE 0xB6

#define BMP280_MODE_SLEEP 0x00
#define BMP280_MODE_FORCED 0x01
#define BMP280_MODE_NORMAL 0x03

#define BMP280_OVERSAMPLE_PRESSURE_SKIPPED 0x00 //Disables pressure measurement.
#define BMP280_OVERSAMPLE_PRESSURE_X1 0x04		//16 bit-> 2.62Pa.
#define BMP280_OVERSAMPLE_PRESSURE_X2 0x08
#define BMP280_OVERSAMPLE_PRESSURE_X4 0x0C
#define BMP280_OVERSAMPLE_PRESSURE_X8 0x10
#define BMP280_OVERSAMPLE_PRESSURE_X16 0x14		//20 bit-> 0.16Pa.

#define BMP280_OVERSAMPLE_TEMPERATURE_SKIPPED 0x00 //Disables temperature measurement.
#define BMP280_OVERSAMPLE_TEMPERATURE_X1 0x20	//16 bit-> 0.0050C.
#define BMP280_OVERSAMPLE_TEMPERATURE_X2 0x40
#define BMP280_OVERSAMPLE_TEMPERATURE_X4 0x60
#define BMP280_OVERSAMPLE_TEMPERATURE_X8 0x80
#define BMP280_OVERSAMPLE_TEMPERATURE_X16 0xA0  //20 bit-> 0.0003C.

#define BMP280_FILTER_OFF 0x00
#define BMP280_FILTER_2 0x04	//Lowest noise reduction.
#define BMP280_FILTER_4 0x08
#define BMP280_FILTER_8 0x0C
#define BMP280_FILTER_16 0x10	//Highest noise reduction.

#define BMP280_STANDBY_0_5_MS 0x00
#define BMP280_STANDBY_62_5_MS 0x01
#define BMP280_STANDBY_125_MS 0x02
#define BMP280_STANDBY_250_MS 0x03
#define BMP280_STANDBY_500_MS 0x04
#define BMP280_STANDBY_1000_MS 0x05
#define BMP280_STANDBY_2000_MS 0x06
#define BMP280_STANDBY_4000_MS 0x07

//Measurement registers.
#define BMP280_PRESSURE_MSB 0xF7
#define BMP280_PRESSURE_LSB 0xF8
#define BMP280_PRESSURE_XLSB 0xF9
#define BMP280_TEMPERATURE_MSB 0xFA
#define BMP280_TEMPERATURE_LSB 0xFB
#define BMP280_TEMPERATURE_XLSB 0xFC

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

//Container for coefficient data.
struct bmp280_coefficients{
	//Temperature.
	uint16_t t_1;
	int16_t t_2;
	int16_t t_3;
	
	//Pressure.
	uint16_t p_1;
	int16_t p_2;
	int16_t p_3;
	int16_t p_4;
	int16_t p_5;
	int16_t p_6;
	int16_t p_7;
	int16_t p_8;
	int16_t p_9;
};

typedef struct bmp280_coefficients bmp280_coefficient_container;

//Global variables.
extern int32_t bmp280_t_fine;

//Functions.
//Set up the sensor with default settings.
void bmp280_set_default(void);
//Set up the sensor with user specified settings.
void bmp280_set(uint8_t mode, uint8_t oversample_pressure, uint8_t oversample_temperature,  uint8_t iir_filter, uint8_t standby_time);
//Saves coefficient data in the host microcontroller. Must be called once before the first measurement.
void bmp280_get_coefficient_data(bmp280_coefficient_container *coefficents);
//Get temperature as a float in Celsius.
float bmp280_get_temperature(bmp280_coefficient_container *coefficents);
//Get pressure as a float in Pa.
float bmp280_get_pressure(bmp280_coefficient_container *coefficents);
//Get the device ID (0x58).
uint8_t bmp280_get_device_id(void);
//Reset the sensor.
void bmp280_reset(void);
//Under development.
void bmp280_force_measurement(void);
//Under development.
uint8_t bmp280_get_measurement_status(void);
//Under development.
uint8_t bmp280_get_nvs_load_status(void);

/*
Example implementation.

#include "i2c.h"
#include "bmp280.h"

int main(void)
{
	bmp280_coefficient_container *coefficients= malloc(sizeof(bmp280_coefficient_container)* 1);

	i2c_set(I2C_BAUD_RATE(I2C_SCL_CLOCK));
	bmp280_set(BMP280_MODE_NORMAL, BMP280_OVERSAMPLE_PRESSURE_X16, BMP280_OVERSAMPLE_TEMPERATURE_X2, BMP280_FILTER_2, BMP280_STANDBY_250_MS);
	bmp280_get_coefficient_data(coefficients);
	
	while (1)
	{
		float temperature= bmp280_get_temperature(coefficients);
		float pressure= bmp280_get_pressure(coefficients);
	}
}

*/

#endif /* BMP280_H_ */