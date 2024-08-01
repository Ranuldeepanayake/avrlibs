///*
 //* PWM.c
 //*
 //* Created: 04-Oct-18 5:26:56 PM
 //* Author : Ranul Deeepanayake
 //* Demonstrates the use of PWM using timer 1.
 //* 
 //*/ 
//
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include "pwm.h"
//
//ISR(TIMER1_COMPA_vect){
	//PORTB^= 0x02;
//}
//
//int main(void)
//{	
	////setTimerPwmPhaseCorrect();
	////setPwmFast();
	////setPwmCtc();
	//setPwmInterrupt();
	//
    //while (1) 
    //{
		////writePwmPhaseCorrect();
		////writePwmFast();
    //}
//}
//
//void setTimerPwmPhaseCorrect(){
	////Method 1. Configure Timer 1 for phase correct PWM @ 25 kHz.
	//DDRB|= 0x06; //Set PWM output pins. 
	//TCCR1A= ((1<< COM1A1) | (1<< COM1B1) | (1<< WGM11));  //Set non-inverted PWM on channel A and B and mode 10: phase correct PWM waveform (TOP = ICR1).
	//TCCR1B= ((1<< WGM13) | (1<< CS10));  //Set waveform generation mode and pre-scaler.
	//ICR1= 319;         // TOP = 319 (25.03 kHz).
	//TCNT1= 0; //Initialize timer.
//}
//
//void writePwmPhaseCorrect(){
	////Set duty cycle.
	//OCR1A= 318;
	//OCR1B= 159;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void setPwmFast(){
	////Method 2. Configure Timer 1 for fast PWM @ 10.63 kHz.
	//DDRB|= 0x06;
	//TCCR1A= 0x82;
	//TCCR1B= 0x19;
	//ICR1= 1500;
	//TCNT1= 0;
//}
//
//void writePwmFast(){
	////Set duty cycle.
	//OCR1A= 150; //The maximum value of OCRnx is the value of ICR. 
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void setPwmCtc(){
	////Method 3.
	//DDRB|= 0x06;
	//TCCR1A= 0x40;
	//TCCR1B= 0x19;
	//ICR1H= 0xF0;
	//ICR1L= 0x00; 
	//TCNT1= 0;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void setPwmInterrupt(){
	////Method 4.
	//DDRB|= 0x06;
	//TCCR1A= 0x03;
	//TCCR1B= 0x11;
	//TIMSK1= (1<< 1); //Enable output compare interrupt.
	////OCR1AH= 0x00;
	////OCR1AL= 0xFF;
	//OCR1AH= 0x00;
	//OCR1AL= 0xFF;
	//sei();
	//TCNT1= 0; 
//}
//
//
//
//
