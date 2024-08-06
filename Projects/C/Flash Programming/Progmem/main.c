/*
 * Flash Programming.c
 *
 * Created: 12-Dec-18 6:14:20 PM
 * Author : Ranul Deepanayake
 * Demonstrates writing to flash memory.
 * Note: When writing to a page in flash memory, the page must be erased first.  
 * Write data in the little endian format (LSB first).
 * The 'word' address is sent to the linker to place the code in the desired bootloader section.
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

//https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

//Variables to test progmem.
PROGMEM const uint8_t  var_a  = 97;
PROGMEM const char var_b[]  = "a";
PROGMEM const int16_t var_c = 4096;

//
PROGMEM const char var_d[]  = "Hello world!\t How are you?";
char var_e[] = "End";

//Strings in progmem.
PROGMEM const char string_1[] = "Gehan";
PROGMEM const char string_2[] = "Damith";
PROGMEM const char string_3[] = "Ben";

//Memory map for the strings above.
PROGMEM const char *const string_table[] = {string_1, string_2, string_3};

//Experimental.	
PROGMEM const char string_array[3][10] = {"XDD", "YDD", "ZDD"};
PROGMEM const char int_array[2][3] = {{0x11, 0x22, 0x23}, {0x11, 0x22, 0x23}, {0x11, 0x22, 0x23}};

int main(void)
{
	uartSet(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);
	char my_char[50];
	
    while (1) 
    {
		/*
		uartPrint("Message: ");
		my_char[0] = pgm_read_byte_near(var_b);
		uartPrintLn(my_char);
		memset(my_char, 0, 50);
		
		uartPrintLn(var_e);
		
		for(uint8_t i = 0; i < strlen_P(var_d); i++){
			my_char[i] = pgm_read_byte_near(var_d + i);
			//uartPrint(tempChar);
		}
		uartPrintLn(my_char);
		memset(my_char, 0, 50);
		*/
		
		//for(uint8_t i = 0; i < 3; i++){
			//strcpy_P(my_char, (char*) pgm_read_ptr_near(&string_table[i]));
			//uartPrintLn(my_char);
			//memset(my_char, 0, 50);
		//}
		
    }
}
