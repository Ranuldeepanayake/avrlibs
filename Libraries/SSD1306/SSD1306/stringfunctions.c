/*
 * stringfunctions.c
 *
 * Created: 24-Dec-18 11:42:03 AM
 * Author: Ranul Deepanayake
 */ 
#include "stringfunctions.h"

void sprintf2(int number, char string[]){
	/*
	Convert a number to a char array. The source array must be SIZE+ 1 long to accomodate the null character.
	Can get number of characters either by iteratively modding or dividing.
	Can handle negative numbers. Floating point numbers will be rounded off to integers (both positive and negative).
	memcpy from the standard library is used.
	*/
	
	int isNegative= 0;
	int tempArraySize= 0;
	
	int divisor= 10;
	int result= 0;
	int digits= 0;
	
	int modDivisor= 1;
	int modDivisorResult= 0;
	
	//Check for negative input;
	if(number< 0){
		isNegative= 1;
		number= number+ (number* -2);
	}
	
	do{
		//Append the individual characters to the array.
		modDivisorResult= number/ modDivisor;
		string[digits]= (modDivisorResult% 10)+ 48;	//Store characters in array. Take into account the post increment done earlier. //Store the modded value in the array. Add 48 (ASCII: 0) for ASCII conversion.
		modDivisor*= 10;																											//Numbers are appended to the string in reverse order.
		//printf("%d\n", string[digits]); //In ASCII decimal.
		
		//Calculate the number of characters.
		result= number/ divisor;
		digits++;
		divisor*= 10;
		
	}while(result> 0);
	
	if(isNegative== 0){
		tempArraySize= digits+ 1;	//Accomodate the null character.
		}else{
		tempArraySize= digits+ 2;	//Accomodate the null character and negative sign.
	}
	
	char temp[tempArraySize];
	memcpy(temp, string, digits);	//Copy the array to another array.

	for(int i= 0; i< digits; i++){
		if(isNegative== 0){
			string[i]= temp[(digits- 1)- i];
			if((i+ 1)== digits){
				string[i+ 1]= 0;	//Append the null character.
			}
			}else{
			if(i== 0){
				string[i]= 45;		//Append the negative sign.
				++i;
			}
			string[i]= temp[digits- i];
			if((i+ 1)== digits){
				string[i+ 1]= temp[digits- (i+ 1)];		//Handle the last loop cycle execution.
			}
			if((i+ 1)== digits){
				string[i+ 2]= 0;	//Append the null character.
			}
		}
	}
}