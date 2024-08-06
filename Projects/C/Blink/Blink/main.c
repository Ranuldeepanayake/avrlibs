/*
 * Blink.c
 *
 * Created: 28-Sep-18 11:41:14 AM
 * Author : Ranul Deepanayake
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

void setLed();
void blinkLed() ;

uint8_t EEMEM eeprom_1[] = {0xCC, 0xCC, 0xCC, 0xCC};
	
__attribute__ ((section (".my_section_1"))) const uint8_t F_MAP[] PROGMEM = {0xDD, 0xDD, 0xDD, 0xDD};

int main(void) 
{
	setLed();
	
    while (1) 
    {
		_delay_ms(500);
		blinkLed();
		_delay_ms(500);
		//uint8_t a = F_MAP[0];
		//a++;
		//__asm__("jmp 0x000200");
		//__attribute__ ((section (".my_section_2"))) (__asm__("jmp 0x000100")))
    }
}

void setLed(){
	DDRB|= 0x20;
}

void blinkLed(){
	PORTB^= 0x20;
}

__attribute__ ((naked)) __attribute__ ((section (".my_section_2"))) void function_1(){
	__asm__("jmp 0x000100");
}

//__attribute__ ((naked)) __attribute__ ((section (".bootloader"))) //Place before the function prototype.
//__attribute__ ((section (".my_section_1"))) //Place before a variable/constant.