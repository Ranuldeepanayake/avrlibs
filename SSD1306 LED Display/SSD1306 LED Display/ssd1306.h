/*
 * ssd1306.h
 *
 * Created: 22-Dec-18 11:59:38 AM
 * Author: Ranul Deepanayake
 * Library for the SSD1306 controller based OLED display.
 * Uses a local frame buffer of 1024 Bytes.
 * Stores an ASCII table, initialization values and test buffer in program flash memory.
 */ 


#ifndef SSD1306_H_
#define SSD1306_H_

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "i2c.h"
#include <string.h>

//Commands and attributes.
#define SSD1306_SLAVE_ADDRESS 0x3C
#define SSD1306_CONTROL_BYTE_COMMAND_SINGLE 0x80	//The C bit determines the number of bytes while the D bit determines the type of byte (command or data).
#define SSD1306_CONTROL_BYTE_COMMAND_STREAM 0x00	//C= 0: stream, C= 1: single, D= 0: command, D= 1: data. 
#define SSD1306_CONTROL_BYTE_DATA_SINGLE 0xC0
#define SSD1306_CONTROL_BYTE_DATA_STREAM 0x40
#define SSD1306_CONTROL_BYTE_CD 0x40
#define SSD1306_DISPLAY_ON 0xAF
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_RAM 0xA4
#define SSD1306_DISPLAY_FORCED 0xA5 
#define SSD1306_DISPLAY_NORMAL 0xA6
#define SSD1306_DISPLAY_INVERTED 0xA7
#define	SSD1306_DISPLAY_CONTRAST 0x81
#define	SSD1306_DISPLAY_CONTRAST_RESET 0x7F 
#define SSD1306_DISPLAY_COLUMN_ADDRESS 0x21
#define SSD1306_DISPLAY_PAGE_ADDRESS 0x22

#define SSD1306_HEIGHT 64
#define SSD1306_WIDTH 128
#define SSD1306_PAGE_COUNT 8
#define SSD1306_PAGE_SIZE 128	//In bits.
#define SSD1306_GDDRAM_SIZE 1024 //In bits.
#define SSD1306_MAX_BRIGHTNESS 255

//Status and error codes.
#define SSD1306_COORDINATE_OUT_OF_RANGE 10
#define SSD1306_SUCCESS 1

#define SSD1306_GDDRAM_TEST_BUFFER_SET

//Functions.
//SSD1306 OLED display initialization sequence.
void ssd1306Initialize();
//Send a single command byte to the display.
void ssd1306SendCommand(uint8_t command);
//Send a single data byte to the GDDRAM.
void ssd1306SendData(uint8_t data);
//Set the X and Y axes in the GDDRAM.
uint8_t ssd1306LocationXY(uint8_t x, uint8_t y);
//Draw a single pixel on the display according to a X and Y axes.
uint8_t ssd1306DrawPixel(uint8_t x, uint8_t y);
//Helper function to select the whole GDDRAM.
void ssd1306SelectWholeBuffer();
//Zero fill the display.
void ssd1306ClearDisplay();
//Fill the GDDRAM with the local frame buffer.
void ssd1306TransferBuffer();
//Tests the font collection stored in flash memory.
void ssd1306TestFont();
//Sends an array of characters to the local display buffer starting from the specified X and Y pixel coordinates.
uint8_t ssd1306Print(char *string, uint8_t x, uint8_t y);
//Fill the local frame buffer with th specified byte pattern.
void ssd1306FillBuffer(uint8_t pattern);
//Control the display contrast. Values between 0x00- 0xFF are accepted.
void ssd1306SetContrast(uint8_t contrast);

//External variables and constants.
extern const uint8_t ssd1306_initialization_values[24] PROGMEM;
extern const char ssd1306_font[128][6] PROGMEM;
#ifdef SSD1306_GDDRAM_TEST_BUFFER_SET
extern const uint8_t ssd1306_gddram_test[1024] PROGMEM; 
#endif

#endif /* SSD1306_H_ */