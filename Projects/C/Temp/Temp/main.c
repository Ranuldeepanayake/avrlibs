#include <avr/io.h>
#include <usart328P.h>
#include <util/delay.h>

int main(){
	
	setUsart(9600);
	setUsartLed(1);
	
	char input;
	char *arr;
	
	while(1){
		arr= "";
		input= readChar();
		*arr= input; 
		println(arr);
		_delay_ms(500);	
	}
}