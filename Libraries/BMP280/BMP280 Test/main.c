/*
 * BMP280.c
 *
 * Created: 04-Nov-18 8:56:40 AM
 * Author : Ranul Deepanayake
 */ 



//Only required for the main function.
//#include "usart328P.h"
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "bmp280.h"
#include "i2c.h"

int main(void)
{
	//setUsart(9600);
	
	i2cSetMaster(I2C_PRESCALER, I2C_BAUD_RATE);
	//char text[20];
	//int device_id= 0;
	
	bmp280SetDefault();
	
	bmp280_coefficient_container *coefficients= malloc(sizeof(bmp280_coefficient_container)* 1);
	bmp280GetCalibrationData(coefficients);
	
    while (1) 
    {
		//device_id= bmp280GetDeviceId();
		/*sprintf(text, "%d", coefficients->t1);
		println(text);
		sprintf(text, "%d", coefficients->t2);
		println(text);
		sprintf(text, "%d", coefficients->t3);
		println(text);*/
		//uint16_t tot= (uint16_t) bmp280GetTemperature(coefficients);
		//uint8_t frac= tot% 100;
		//uint8_t dec= tot/ 100;
		//sprintf(text, "%d.%d", dec, frac);
		//println(text);
		//_delay_ms(500);
    }
}

