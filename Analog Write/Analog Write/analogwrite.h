/*
 * analogwrite.h
 *
 * Created: 05-Feb-19 9:04:53 PM
 * Author: Ranul Deepanayake
 * PWM and frequency generator library for the ATmega328P running at 16MHz. Might work for other AVRs.
 * Support for 8MHz and 1MHz will be added later.
 * Timer 0, 1, and 2 can be used for PWM generation.
 * Timer 0 and 2 have a PWM resolution of 8 bits (0- 255).
 * Timer 1 running in 8 bit mode has a resolution of 8 bits (0- 255) while 10 bit mode has a resolution of 10 bits (0- 1023).
 * Timer 1 can be used for frequency generation (uses prescaler 1).
 * Please study the mode defines carefully.
 * The DDR registers of the respective OCR pins have to be set up to see an output on the pins.
 */ 


#ifndef ANALOGWRITE_H_
#define ANALOGWRITE_H_

//Includes
#include <avr/io.h>

//Defines.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define F_CPU_16_MHZ 0
#define F_CPU_8_MHZ 1
#define F_CPU_1_MHZ 2

//Select according to the system clock.
#if F_CPU== 16000000UL
#define F_CPU_VALUE F_CPU_16_MHZ
#elif F_CPU== 8000000UL
#define F_CPU_VALUE F_CPU_8_MHZ
#elif F_CPU== 1000000UL
#define F_CPU_VALUE F_CPU_1_MHZ
#endif

//Common.
#define TIMER_0 0
#define TIMER_1 1 
#define TIMER_2 2
#define OCR_A 0
#define OCR_B 1

#define TIMER_COMPARE_A_OUTPUT_MODE_DISCONNECTED 0x00
#define TIMER_COMPARE_A_OUTPUT_MODE_NON_INVERTED 0x80
#define TIMER_COMPARE_A_OUTPUT_MODE_INVERTED 0xC0

#define TIMER_COMPARE_B_OUTPUT_MODE_DISCONNECTED 0x00
#define TIMER_COMPARE_B_OUTPUT_MODE_NON_INVERTED 0x20
#define TIMER_COMPARE_B_OUTPUT_MODE_INVERTED 0x30

//Timer 0 and 2 (8 bit). 
#define TIMER_0_PRESCALER_SELECT_REGISTER TCCR0B
#define TIMER_2_PRESCALER_SELECT_REGISTER TCCR2B

#define TIMER8_WAVEFORM_GENERATION_NORMAL 0x00
#define TIMER8_WAVEFORM_GENERATION_PHASE_CORRECT 0x01
#define TIMER8_WAVEFORM_GENERATION_FAST 0x03


//Timer 0 and 2 prescaler settings for a 16MHz system clock.
//TImer 0.
#define TIMER8_16_CLCK_62_KHZ 0
#define TIMER8_16_CLCK_7_KHZ 1
#define TIMER8_16_CLCK_976_HZ 2
#define TIMER8_16_CLCK_244_HZ 3
#define TIMER8_16_CLCK_61_HZ 4

//Timer 2.
#define TIMER8_2_16_CLCK_62_KHZ 5
#define TIMER8_2_16_CLCK_7_KHZ 6
#define TIMER8_2_16_CLCK_2_KHZ 7
#define TIMER8_2_16_CLCK_976_HZ 8
#define TIMER8_2_16_CLCK_488_HZ 9
#define TIMER8_2_16_CLCK_244_HZ 10
#define TIMER8_2_16_CLCK_61_HZ 11

#define TIMER8_16_CLCK_62_KHZ_PRESCALER_BITS 0x01
#define TIMER8_16_CLCK_7_KHZ_PRESCALER_BITS 0x02
#define TIMER8_16_CLCK_976_HZ_PRESCALER_BITS 0x03
#define TIMER8_16_CLCK_244_HZ_PRESCALER_BITS 0x04
#define TIMER8_16_CLCK_61_HZ_PRESCALER_BITS 0x05

#define TIMER8_2_16_CLCK_62_KHZ_PRESCALER_BITS 0x01
#define TIMER8_2_16_CLCK_7_KHZ_PRESCALER_BITS 0x02
#define TIMER8_2_16_CLCK_2_KHZ_PRESCALER_BITS 0x03
#define TIMER8_2_16_CLCK_976_HZ_PRESCALER_BITS 0x04
#define TIMER8_2_16_CLCK_488_HZ_PRESCALER_BITS 0x05
#define TIMER8_2_16_CLCK_244_HZ_PRESCALER_BITS 0x06
#define TIMER8_2_16_CLCK_61_HZ_PRESCALER_BITS 0x07

//Timer 0 and 2 prescaler settings for an 8MHz system clock.
#define TIMER8_8_CLCK_8_MHZ 12
#define TIMER8_8_CLCK_1_MHZ 13
#define TIMER8_8_CLCK_125_KHZ 14
#define TIMER8_8_CLCK_31_KHZ 15
#define TIMER8_8_CLCK_7_KHZ 16


//Timer 1 (16 bit).
#define TIMER_1_PRESCALER_SELECT_REGISTER TCCR1B

#define TIMER16_WAVEFORM_GENERATION_NORMAL 0
#define TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_16_BIT 1
#define TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_8_BIT 2
#define TIMER16_WAVEFORM_GENERATION_FAST_16_BIT 3
#define TIMER16_WAVEFORM_GENERATION_FAST_8_BIT 4

#define TIMER16_WAVEFORM_GENERATION_NORMAL_SELECTOR (TCCR1A|= 0x00);
#define TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_16_BIT_SELECTOR (TCCR1A|= 0x03);
#define TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_8_BIT_SELECTOR (TCCR1A|= 0x01);
#define TIMER16_WAVEFORM_GENERATION_FAST_16_BIT_SELECTOR (TCCR1A|= 0x03); (TCCR1B|= 0x08);
#define TIMER16_WAVEFORM_GENERATION_FAST_8_BIT_SELECTOR (TCCR1A|= 0x01); (TCCR1B|= 0x08);

//Timer 1 prescaler settings for a 16MHz system clock.
#define TIMER16_16_CLCK_16_bit_15_KHZ 0
#define TIMER16_16_CLCK_16_bit_2_KHZ 1
#define TIMER16_16_CLCK_16_bit_244_HZ 2
#define TIMER16_16_CLCK_16_bit_61_HZ 3
#define TIMER16_16_CLCK_16_bit_15_HZ 4

#define TIMER16_16_CLCK_8_bit_62_KHZ 5
#define TIMER16_16_CLCK_8_bit_7_KHZ 6
#define TIMER16_16_CLCK_8_bit_976_HZ 7
#define TIMER16_16_CLCK_8_bit_244_HZ 8
#define TIMER16_16_CLCK_8_bit_61_HZ 9

#define TIMER16_16_CLCK_16_bit_15_KHZ_PRESCALER_BITS 0x01
#define TIMER16_16_CLCK_16_bit_2_KHZ_PRESCALER_BITS 0x02
#define TIMER16_16_CLCK_16_bit_244_HZ_PRESCALER_BITS 0x03
#define TIMER16_16_CLCK_16_bit_61_HZ_PRESCALER_BITS 0x04
#define TIMER16_16_CLCK_16_bit_15_HZ_PRESCALER_BITS 0x05

#define TIMER16_16_CLCK_8_bit_62_KHZ_PRESCALER_BITS 0x01
#define TIMER16_16_CLCK_8_bit_7_KHZ_PRESCALER_BITS 0x02
#define TIMER16_16_CLCK_8_bit_976_HZ_PRESCALER_BITS 0x03
#define TIMER16_16_CLCK_8_bit_244_HZ_PRESCALER_BITS 0x04
#define TIMER16_16_CLCK_8_bit_61_HZ_PRESCALER_BITS 0x05

//Timer 1 prescaler settings for an 8MHz system clock.
#define TIMER16_8_CLCK_8_MHZ 10
#define TIMER16_8_CLCK_1_MHZ 11
#define TIMER16_8_CLCK_125_KHZ 12
#define TIMER16_8_CLCK_31_KHZ 13
#define TIMER16_8_CLCK_7_KHZ 14

//Frequency generation.
#define TIMER16_FREQUENCY_WAVEFORM_GENERATION_CTC 0
#define TIMER16_FREQUENCY_WAVEFORM_GENERATION_PHASE_AND_FREQUENCY_CORRECT 1
#define TIMER16_FREQUENCY_WAVEFORM_GENERATION_FAST 2

//Functions.
//Set up PWM on the specified port and OCR pin.
void analog_write_pwm_set(uint8_t timer, uint8_t frequency, uint8_t waveform, uint8_t ocr_mode);
//Write PWM value to output compare pin.
void analog_write_pwm_write(uint8_t timer, uint8_t ocr, uint16_t duty_cycle);
//Set up frequency generator.
void analog_write_frequency_set(uint8_t waveform);
//Generate the specified frequency and duty cycle.
void analog_write_frequency_write(uint8_t waveform, uint32_t frequency, uint16_t duty_cycle);

/*
Example implementation.

//PWM with fixed frequencies.
#include "analogwrite.h"

int main(void)
{
	DDRD|= 0xFF;
	DDRB|= 0xFF;
	
	analog_write_pwm_set(TIMER_0, TIMER8_16_CLCK_244_HZ, TIMER8_WAVEFORM_GENERATION_FAST, TIMER_COMPARE_A_OUTPUT_MODE_NON_INVERTED);
	analog_write_pwm_write(TIMER_0, OCR_A, 150);
	
	analog_write_pwm_set(TIMER_0, TIMER8_16_CLCK_244_HZ, TIMER8_WAVEFORM_GENERATION_FAST, TIMER_COMPARE_B_OUTPUT_MODE_NON_INVERTED);
	analog_write_pwm_write(TIMER_0, OCR_B, 25);
	
	analog_write_pwm_set(TIMER_1, TIMER16_16_CLCK_16_bit_61_HZ, TIMER8_WAVEFORM_GENERATION_FAST, TIMER_COMPARE_A_OUTPUT_MODE_NON_INVERTED);
	analog_write_pwm_write(TIMER_1, OCR_A, 512);
	
	analog_write_pwm_set(TIMER_1, TIMER16_16_CLCK_16_bit_61_HZ, TIMER8_WAVEFORM_GENERATION_FAST, TIMER_COMPARE_B_OUTPUT_MODE_NON_INVERTED);
	analog_write_pwm_write(TIMER_1, OCR_B, 256);
	
	analog_write_pwm_set(TIMER_2, TIMER8_2_16_CLCK_7_KHZ, TIMER8_WAVEFORM_GENERATION_FAST, TIMER_COMPARE_A_OUTPUT_MODE_NON_INVERTED);
	analog_write_pwm_write(TIMER_2, OCR_A, 75);
	
	analog_write_pwm_set(TIMER_2, TIMER8_2_16_CLCK_7_KHZ, TIMER8_WAVEFORM_GENERATION_FAST, TIMER_COMPARE_B_OUTPUT_MODE_NON_INVERTED);
	analog_write_pwm_write(TIMER_2, OCR_B, 200);

	while (1)
	{
		
	}
}

//PWM with variable frequencies.
#include "analogwrite.h"

int main(void)
{
	DDRB|= 0xFF;
	
	analog_write_frequency_set(TIMER16_FREQUENCY_WAVEFORM_GENERATION_PHASE_AND_FREQUENCY_CORRECT);
	
	while (1)
	{
		analog_write_frequency_write(TIMER16_FREQUENCY_WAVEFORM_GENERATION_PHASE_AND_FREQUENCY_CORRECT, 25000, 175);
	}
}
*/

#endif /* ANALOGWRITE_H_ */