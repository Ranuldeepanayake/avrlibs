/*
 * SSD1306 LED Display.c
 *
 * Created: 03-Dec-18 12:59:14 PM
 * Author : Ranul Deepanayake
 * Testing ground for the SSD1306 display library.
 */ 

#include "ssd1306.h"
#include "stringfunctions.h"
#include <util/delay.h>

//Draw four pixels on the four corners of the display.
void testCorners();
//Print hello world.
void testHelloWorld();
//Display the incremented number on the display.
void testCounter();

int main(void)
{
	i2cSet(I2C_PRESCALER, I2C_BAUD_RATE);
	ssd1306Initialize();
	testHelloWorld();
	
    while (1) 
    {
		
    }
	
}

void testCorners(){
	/*
	Draw four pixels on the four corners of the display.
	Doesn't use the local frame buffer.
	*/
	ssd1306ClearDisplay();
	ssd1306TransferBuffer();
	ssd1306DrawPixel(0, 0);
	ssd1306DrawPixel(127, 0);
	ssd1306DrawPixel(0, 63);
	ssd1306DrawPixel(127, 63);
}

void testHelloWorld(){
	/*
	Print hello world.
	Uses the local frame buffer.
	*/
	ssd1306ClearDisplay();
	ssd1306Print("Hello world! How are you?", 0, 0);	//The null character is automatically appended.
	ssd1306TransferBuffer();
}

void testCounter(){
	/*
	Display the incremented number on the display.
	Uses the local frame buffer.
	*/
	ssd1306ClearDisplay();
	for(uint16_t i= 0; i< 65535; i++){
		char temp[6];
		sprintf2(i, temp);
		ssd1306Print("Counter: ", 0, 32);
		ssd1306Print(temp, 64, 32);
		ssd1306TransferBuffer();
		_delay_ms(25);
	}
}

