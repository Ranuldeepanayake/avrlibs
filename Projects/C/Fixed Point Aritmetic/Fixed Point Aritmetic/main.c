/*
 * Fixed Point Aritmetic.c
 *
 * Created: 21-Nov-18 11:32:22 AM
 * Author : Ranul Deepanayake
 */ 

#include <avr/io.h>
#include <math.h>


int main(void)
{
    //float a= 10.5;	//Uses more memory and cycles.
	//uint8_t b= 10;
	int16_t decimal= 4;
	uint32_t fraction= 4.8* (pow(2, 16));
	uint32_t operand= 12.5* (pow(2, 16));
	uint32_t result= 0;
	
	result= (operand/ fraction);
	 
    while (1) 
    {
		//a= a/3; //Takes 500 cycles compared to less than 10 for a uint8_t!
		//b= b/3;	
		decimal= 4;
    }
}

