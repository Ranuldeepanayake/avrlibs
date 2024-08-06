/*
 * Tone Generation.c
 *
 * Created: 08-Dec-18 6:56:14 PM
 * Author : Ranul Deepanayake
 * Demonstrates playing the happybirthday song using Timer 1.
 * The melody table has been forced to reside in the program space and is not loaded to RAM.
 */ 

//512 and 54.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define MELODY_DELAY 650U
#define MELODY_COUNT 25U
#define F_IO 16000000UL
#define PRESCALER 1

void setMillisecondTimer();
uint16_t getMilliseconds();
void setToneGenerator();
void setTone(uint16_t tone);
uint16_t getOCRnx(uint16_t frequecy);

volatile uint16_t milliseconds= 0;

ISR(TIMER0_COMPA_vect){
	milliseconds++;
}

const uint16_t melody[25] PROGMEM= {262, 294, 262, 349, 330, 330, 262, 294, 262, 392, 349, 349, 262, 523, 440, 349, 330, 294, 294, 
	466, 440, 349, 392, 349, 349};	//In Hertz.
/*uint16_t melody[25]= {30450,  27150, 30450, 22870, 24185, 24185, 30450, 27150, 30450, 20360, 22870, 22870, 30450, 15260, 18140, 22870, 24185, 27150, 27150,
17127, 18140, 22870, 20360, 22870, 22870};*/
	
uint16_t previous_time= 0, current_time= 0;

int main(void)
{
	uint8_t melody_incrementer= 0;
	
	setMillisecondTimer();
	setToneGenerator();
	sei();
	
    while (1) 
    {	
		if(getMilliseconds()- previous_time>= MELODY_DELAY){
			previous_time= getMilliseconds();
			melody_incrementer== MELODY_COUNT ? (melody_incrementer= 0) : (melody_incrementer= melody_incrementer);	//Wrap around.
			setTone(getOCRnx(pgm_read_word(&(melody[melody_incrementer]))));
			melody_incrementer++;
		}
			
		/*for(int i= 0; i<10; i++){
			setTone(melody[i]);
			_delay_ms(250);
		}*/
    }
}

void setMillisecondTimer(){
	//Count milliseconds. Prescaler: 64, CTC and OCR0A disconnected.
	TCCR0A= 0x02;
	TCCR0B= 0x03;
	TIMSK0= 0x02;
	OCR0A= 249;
	TCNT0= 0x00;
}

uint16_t getMilliseconds(){
	cli();
	uint16_t temp= milliseconds;
	sei();
	return temp;
}

void setToneGenerator(){
	//Prescaler: 1 and CTC toggle with OCRnx as TOP.
	DDRB|= 0xFF;
	TCCR1A= 0x40;
	TCCR1B= 0x09;
	OCR1A= 0x00;	//7974- 7981 1KHz.
	TCNT1= 0x0000;
}

void setTone(uint16_t tone){
	OCR1AH= tone>> 8;
	OCR1AL= tone;
}

uint16_t getOCRnx(uint16_t frequecy){
	//Converts a given frequency to an output compare value.
	return (F_IO/(frequecy* 2* PRESCALER))- 1;
}
