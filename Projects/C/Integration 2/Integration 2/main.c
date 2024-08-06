/*
 * Integration 2.c
 *
 * Created: 24-Dec-18 1:17:46 PM
 * Author : Ranul Deepanayake
 */ 

#include "integration2.h"

volatile uint16_t sensor_0= 0, sensor_1= 0;
volatile uint8_t sensor_number= 0;

ISR(ADC_vect){
	if(sensor_number== 0){
		sensor_0= ADCL;
		sensor_0|= ADCH<< 8;
		sensor_number>15 ? (sensor_number= 0) : (sensor_number= sensor_number);		//Guard.
		sensor_number++;															//Increment ADC channel.
		ADMUX&= 0xF0;																//Clear the MUX bits.
		ADMUX|= sensor_number;														//Append new MUX bits.
	}else{
		sensor_1= ADCL;
		sensor_1|= ADCH<< 8;
		sensor_number= 0;															//Wrap around.
		sensor_number>15 ? (sensor_number= 0) : (sensor_number= sensor_number);		//Guard.
		ADMUX&= 0xF0;																//Clear the MUX bits.
		ADMUX|= sensor_number;														//Append new MUX bits.
	}
}

//The container of the buffer.
typedef struct{
	uint16_t buffer[10]; //Int array.
	uint8_t head;
	uint8_t tail;
	uint8_t maxLen;	//Size of the buffer in number of number of elements.
}circularBuffer;

uint8_t push(circularBuffer *buff, uint16_t data);
uint8_t pop(circularBuffer *buff, uint16_t *data);

int main(void)
{
	circularBuffer sensor_0_que_buffer;
	circularBuffer *sensor_0_que= &sensor_0_que_buffer;
	circularBuffer sensor_1_que_buffer;
	circularBuffer *sensor_1_que= &sensor_1_que_buffer;
	sensor_0_que->head= 0;
	sensor_0_que->tail= 0;
	sensor_0_que->maxLen= 10;
	sensor_1_que->head= 0;
	sensor_1_que->tail= 0;
	sensor_1_que->maxLen= 10;
	uint16_t data= 0;
	
	i2cSet(I2C_PRESCALER, I2C_BAUD_RATE);
	ssd1306Initialize();
	ssd1306ClearDisplay();
	ssd1306Print("Main loaded", 0, 0);
	ssd1306TransferBuffer();
	char string[6];
	
	setADC();
	sei();
	
    while (1) 
    {
		push(sensor_0_que, sensor_0);
		push(sensor_1_que, sensor_1);
		
		pop(sensor_0_que, &data);
		sprintf2(data, string);
		ssd1306Print("Sensor 0 Value:", 0, 0);
		ssd1306Print("     ", 96, 0);
		ssd1306Print(string, 96, 0);
		
		pop(sensor_1_que, &data);
		sprintf2(data, string);
		ssd1306Print("Sensor 1 Value:", 0, 32);
		ssd1306Print("     ", 96, 32);
		ssd1306Print(string, 96, 32);
		ssd1306TransferBuffer();
		
		ssd1306SetContrast(sensor_0>> 2);
    }
}

uint8_t push(circularBuffer *buff, uint16_t data){
	//It is a good practice to insert new data before incrementing the head.
	//'->' used since a pointer is being used.
	uint8_t next= 0;
	next= buff-> head+ 1;	//Where the head will point after this write.
	
	if(next>= buff-> maxLen){	//Overwrite the first (oldest) element when the buffer is full (only if it has been read though. See the below function).
		next= 0;
	}
	
	if(next== buff-> tail){
		return 1;	//Discard write if buffer hasn't been read (popped).
	}
	
	buff-> buffer[buff-> head]= data;	//Insert data into the buffer at the current head position.
	buff-> head= next;	//Increment the head position by one.

	return 0;	//Return on success.
}

uint8_t pop(circularBuffer *buff, uint16_t *data){
	//It is a good practice to read old data before incrementing the tail.
	int next= 0;
	
	if(buff-> head== buff-> tail){	//If head== tail, the buffer is empty and therefore shouldn't be read.
		return 1;
	}
	next= buff-> tail+ 1;	//Where the tail will point to after this read.
	
	if(next>= buff-> maxLen){	//Start from the beginning if all positions of the buffer have been read from.
		next= 0;
	}
	*data= buff-> buffer[buff-> tail];	//Read data.
	buff-> tail= next;	//Increment tail.
	
	return 0;	//Return on success.
}


