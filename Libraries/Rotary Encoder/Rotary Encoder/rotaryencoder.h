/*
 * rotaryencoder.h
 *
 * Created: 31-Jan-19 12:14:04 PM
 * Author: Ranul Deepanayake
 * Rotary encoder library for the ATmega328P.
 * Supports manual polling and interrupt polling.
 * Supports pushbutton counter reset.
 * Supports rotary encoders which outputs a logic low signal for a pulse. 
 * Please use the interrupt based implementation for accurate results.
 * Please change the register and pin defines according to your setup.
 * Pin CLK-> PIN A, pin DT-> PIN B.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef ROTARYENCODER_H_
#define ROTARYENCODER_H_

//Attributes.
#define ROTARY_ENCODER_DDR_REGISTER DDRD
#define ROTARY_ENCODER_PIN_REGISTER PIND
#define ROTARY_ENCODER_PCICR_BANK 0x04
#define ROTARY_ENCODER_PCMSK_REGISTER PCMSK2

#define ROTARY_ENCODER_SWITCH_PIN 0x20
#define ROTARY_ENCODER_INPUT_A_PIN 0x40
#define ROTARY_ENCODER_INPUT_B_PIN 0x80

#define ROTARY_ENCODER_READ_SWITCH (ROTARY_ENCODER_PIN_REGISTER & ROTARY_ENCODER_SWITCH_PIN)
#define ROTARY_ENCODER_READ_INPUT_A (ROTARY_ENCODER_PIN_REGISTER & ROTARY_ENCODER_INPUT_A_PIN)
#define ROTARY_ENCODER_READ_INPUT_B (ROTARY_ENCODER_PIN_REGISTER & ROTARY_ENCODER_INPUT_B_PIN)

#define ROTARY_ENCODER_INPUT_HIGH 1
#define ROTARY_ENCODER_INPUT_LOW 0

#define ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_0 0x01
#define ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_0 0x02
#define ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_1 0x04
#define ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_1 0x08
#define ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_2  0x10
#define ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_2 0x20

//Functions.
//Set up rotary encoder polling using pin change interrupts.
void rotary_encoder_set_interrupt();
//Set up rotary encoder polling manually.
void rotary_encoder_set();
//Return counter value.
uint8_t rotary_encoder_read_interrupt();
//Increment or decrement counter by manual polling.
void rotary_encoder_read();

//External variables.
extern uint8_t rotary_encoder_counter;	

/*
Example implementation.

#include "rotaryencoder.h"

int main(void)
{
	rotary_encoder_set_interrupt();

	while (1)
	{
		uint8_t temp_counter= rotary_encoder_read_interrupt();
		//Do stuff.
	}
}

*/

#endif /* ROTARYENCODER_H_ */