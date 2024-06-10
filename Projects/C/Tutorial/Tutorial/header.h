/*
 * header.h
 *
 * Created: 29-Sep-18 9:17:02 PM
 *  Author: ranul
 */ 


#ifndef HEADER_H_
#define HEADER_H_

#define shiftLeft(byte) (1<< byte)
#define shiftRight(byte) (1>> byte)

void blink13();
void blink13Advanced();
void blinkAllSet();
void blinkAll();
void fadeSingle();
void pattern();
void patternBoth();


#endif /* HEADER_H_ */