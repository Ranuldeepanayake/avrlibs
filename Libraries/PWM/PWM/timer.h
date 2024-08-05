/*
 * timer.h
 *
 * Created: 04-Aug-24 5:26:56 PM
 * Author : Ranul Deeepanayake
 * A timer library for the ATmega 328P.
 * Supports timer 0/1/2.
 * Uses timer 0/1 for waveform generation (PWM and frequency).
 * Uses timer 2 for millis and micros functions.
 * Supported operational modes: Normal, CTC, fast PWM, phase correct PWM and phase and frequency correct PWM.
 * Supported OCnx pin operating modes: Disabled, toggle, clear on match (non- inverting) and set on match (inverting). 
 * Refer 'timer_common.c' for an in depth comment on the different operational mode.
 * Uses a separate source code page for each timer and one page for common functions.
 */ 

#ifndef PWM_H_
#define PWM_H_

//Output data direction register and pin mapping.
#ifndef MCU
	#define MCU atmega328p
#endif
#if MCU == atmega328p
	//DDR register.
	#define TIMER_0_PIN_A_DDR_REGISTER 0x0A //Pin 12/PD6
	#define TIMER_0_PIN_B_DDR_REGISTER 0x0A //Pin 11/PD5
	#define TIMER_1_PIN_A_DDR_REGISTER 0x04 //Pin 15/PB1
	#define TIMER_1_PIN_B_DDR_REGISTER 0x04 //Pin 16/PB2
	#define TIMER_2_PIN_A_DDR_REGISTER 0x04 //Pin 17/PB3
	#define TIMER_2_PIN_B_DDR_REGISTER 0x0A //Pin  5/PD3
	//Pin number.
	#define TIMER_0_PIN_A (1 << PORTD6)
	#define TIMER_0_PIN_B (1 << PORTD5)
	#define TIMER_1_PIN_A (1 << PORTB1)
	#define TIMER_1_PIN_B (1 << PORTB2)
	#define TIMER_2_PIN_A (1 << PORTB3)
	#define TIMER_2_PIN_B (1 << PORTD3)
#endif
//Timers.
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
//Channels.
#define TIMER_N_CHANNEL_A 0
#define TIMER_N_CHANNEL_B 1
//Prescalers.
#define TIMER_N_PRESCALER_NONE 0x00
#define TIMER_N_PRESCALER_1 0x01
#define TIMER_N_PRESCALER_8 0x02
#define TIMER_N_PRESCALER_64 0x03
#define TIMER_N_PRESCALER_256 0x04
#define TIMER_N_PRESCALER_1024 0x05
#define TIMER_N_PRESCALER_EXTERNAL_FALLING 0x06
#define TIMER_N_PRESCALER_EXTERNAL_RISING 0x07
//Force compare switches.
#define TIMER_N_FORCE_COMPARE_A 0x80
#define TIMER_N_FORCE_COMPARE_B 0x40
//Output channel behavior.
#define TIMER_N_PIN_A_DISCONNECTED 0x00
#define TIMER_N_PIN_A_TOGGLE 0x40
#define TIMER_N_PIN_A_CLEAR_ON_MATCH 0x80
#define TIMER_N_PIN_A_SET_ON_MATCH 0xC0
#define TIMER_N_PIN_B_DISCONNECTED 0x00
#define TIMER_N_PIN_B_TOGGLE 0x10
#define TIMER_N_PIN_B_CLEAR_ON_MATCH 0x20
#define TIMER_N_PIN_B_SET_ON_MATCH 0x30
//Modes.
//Timer 8.
#define TIMER_N_MODE_NORMAL								0
#define TIMER_N_MODE_PWM_PHASE_CORRECT					1
#define TIMER_N_MODE_CTC								2
#define TIMER_N_MODE_PWM_FAST							3
#define TIMER_N_MODE_PWM_PHASE_CORRECT_OCR				4
#define TIMER_N_MODE_PWM_FAST_OCR						5
//Timer 16.
#define TIMER_16_MODE_NORMAL							7
#define TIMER_16_MODE_PWM_PHASE_CORRECT_8_BIT			8
#define TIMER_16_MODE_PWM_PHASE_CORRECT_9_BIT			9
#define TIMER_16_MODE_PWM_PHASE_CORRECT_10_BIT			10
#define TIMER_16_MODE_CTC_OCR							11
#define TIMER_16_MODE_PWM_FAST_8_BIT					12
#define TIMER_16_MODE_PWM_FAST_9_BIT					13
#define TIMER_16_MODE_PWM_FAST_10_BIT					14
#define TIMER_16_MODE_PWM_PHASE_FREQUENCY_CORRECT_ICR	15
#define TIMER_16_MODE_PWM_PHASE_FREQUENCY_CORRECT_OCR	16
#define TIMER_16_MODE_PWM_PHASE_CORRECT_ICR				17
#define TIMER_16_MODE_PWM_PHASE_CORRECT_OCR				18
#define TIMER_16_MODE_CTC_ICR							19
#define TIMER_16_MODE_PWM_FAST_ICR						20 
#define TIMER_16_MODE_PWM_FAST_OCR						21
//Timer 8 WGM[1:0].
#define TIMER_N_WGM_01_NORMAL 0x00
#define TIMER_N_WGM_01_PWM_PHASE_CORRECT 0x01
#define TIMER_N_WGM_01_CTC 0x02
#define TIMER_N_WGM_01_PWM_FAST 0x03
#define TIMER_N_WGM_01_PWM_PHASE_CORRECT_OCR 0x05
#define TIMER_N_WGM_01_PWM_FAST_OCR 0x07
//Timer 16 WGM[1:0].
#define TIMER_16_WGM_01_NORMAL							0x00
#define TIMER_16_WGM_01_PWM_PHASE_CORRECT_8_BIT			0x01
#define TIMER_16_WGM_01_PWM_PHASE_CORRECT_9_BIT			0x02
#define TIMER_16_WGM_01_PWM_PHASE_CORRECT_10_BIT		0x03
#define TIMER_16_WGM_01_CTC_OCR							0x00
#define TIMER_16_WGM_01_PWM_FAST_8_BIT					0x01
#define TIMER_16_WGM_01_PWM_FAST_9_BIT					0x02
#define TIMER_16_WGM_01_PWM_FAST_10_BIT					0x03
#define TIMER_16_WGM_01_PWM_PHASE_FREQUENCY_CORRECT_ICR	0x00
#define TIMER_16_WGM_01_PWM_PHASE_FREQUENCY_CORRECT_OCR	0x01
#define TIMER_16_WGM_01_PWM_PHASE_CORRECT_ICR			0x02
#define TIMER_16_WGM_01_PWM_PHASE_CORRECT_OCR			0x03
#define TIMER_16_WGM_01_CTC_ICR							0x00
#define TIMER_16_WGM_01_PWM_FAST_ICR					0x02
#define TIMER_16_WGM_01_PWM_FAST_OCR					0x03
//Timer 8 WGM[2].
#define TIMER_N_WGM_2_NORMAL 0x00
#define TIMER_N_WGM_2_PWM_PHASE_CORRECT 0x00
#define TIMER_N_WGM_2_CTC 0x00
#define TIMER_N_WGM_2_PWM_FAST 0x00
#define TIMER_N_WGM_2_PWM_PHASE_CORRECT_OCR 0x08
#define TIMER_N_WGM_2_PWM_FAST_OCR 0x08
//Timer 16 WGM[3:2].
#define TIMER_16_WGM_23_NORMAL							0x00
#define TIMER_16_WGM_23_PWM_PHASE_CORRECT_8_BIT			0x00
#define TIMER_16_WGM_23_PWM_PHASE_CORRECT_9_BIT			0x00
#define TIMER_16_WGM_23_PWM_PHASE_CORRECT_10_BIT		0x00
#define TIMER_16_WGM_23_CTC_OCR							0x08
#define TIMER_16_WGM_23_PWM_FAST_8_BIT					0x08
#define TIMER_16_WGM_23_PWM_FAST_9_BIT					0x08
#define TIMER_16_WGM_23_PWM_FAST_10_BIT					0x08
#define TIMER_16_WGM_23_PWM_PHASE_FREQUENCY_CORRECT_ICR	0x10
#define TIMER_16_WGM_23_PWM_PHASE_FREQUENCY_CORRECT_OCR	0x10
#define TIMER_16_WGM_23_PWM_PHASE_CORRECT_ICR			0x10
#define TIMER_16_WGM_23_PWM_PHASE_CORRECT_OCR			0x10
#define TIMER_16_WGM_23_CTC_ICR							0x18
#define TIMER_16_WGM_23_PWM_FAST_ICR					0x18
#define TIMER_16_WGM_23_PWM_FAST_OCR					0x18
//Interrupt switches.
#define TIMER_N_INTERRUPT_NONE 0x00
#define TIMER_N_OVERFLOW_INTERRUPT_ENABLE 0x01
#define TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_ENABLE 0x02
#define TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_ENABLE 0x04
//Interrupt flags.
#define TIMER_N_OVERFLOW_INTERRUPT_FLAG 0x01
#define TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_FLAG 0x02
#define TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_FLAG 0x04
//Millis and micros related.
#define TIMER_MILLIS_OCR 0xF9 //249 counts.
#define TIMER_MICROS_OCR 0x01 //1 count.
#define TIMER_MICROS_MULTIPLIER 8

/*
Functions.
*/
/*
Timer 0.
*/
//Set the timer.
void timer0Set(uint8_t mode, uint8_t output, uint8_t prescaler);
//Write to OCR.
void timer0Write(uint8_t channel, uint8_t value);

/*
Timer 1.
*/
//Set the timer.
void timer1Set(uint8_t mode, uint8_t output, uint8_t prescaler);
//Write to OCR.
void timer1WriteOcr(uint8_t channel, uint8_t value);
//Write to ICR.
void timer1WriteIcr(uint8_t value);

/*
Timer 2.
*/
//Set the timer.
void timer2Set(uint8_t mode, uint8_t output, uint8_t prescaler);
//Write to OCR.
void timer2Write(uint8_t channel, uint8_t value);
//Set timer to count milliseconds.
void timer2SetMillis();
//Set timer to count microseconds.
void timer2SetMicros();
//Get milliseconds.
uint16_t timer2GetMillis();
//Get microseconds.
uint16_t timer2GetMicros();

/*
Common functions.
*/
//Set the pin direction as output for OCn pins.
void timerSetOutputPins(uint8_t timer, uint8_t channel);
//Set the prescaler.
void timerSetPrescaler(uint8_t timer, uint8_t prescaler);
//Enable specific timer interrupts.
void timerEnableInterrupts(uint8_t timer, uint8_t interrupt);
//Disable specific timer interrupts.
void timerDisableInterrupts(uint8_t timer, uint8_t interrupt);

/*

Example implementation for PWM or frequency generation.

int main(void)
{
	adcSet(ADC_MODE_SINGLE_CONVERSION, ADC_PRESCALER_128, ADC_VREF_AVCC);
	timerSetOutputPins(TIMER_2, TIMER_N_CHANNEL_B);
	timer2Set(TIMER_N_MODE_PWM_FAST, TIMER_N_PIN_B_CLEAR_ON_MATCH, TIMER_N_PRESCALER_256);
	
	while (1)
	{
		timer2Write(TIMER_N_CHANNEL_B, (uint8_t) (adcRead(ADC_0) / 4));
	}
}


Example implementation for millis and micros.

int main(void)
{
	uint16_t previous_time= 0, current_time= 0, on_time= 500, total_time= 1000;
	timer2SetMillis();
	DDRB |= 0x20;
	
	while (1)
	{
		current_time= timer2GetMillis();
		if((current_time- previous_time) >= total_time){
			PORTB |= 0x20;
			previous_time = timer2GetMillis();
		}
		
		if(current_time - previous_time >= on_time){
			PORTB &= ~(0x20);
		}
	}
}

*/

#endif /* PWM_H_ */