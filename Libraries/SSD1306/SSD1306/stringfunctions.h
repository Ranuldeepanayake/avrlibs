/*
 * stringfunctions.h
 *
 * Created: 24-Dec-18 11:42:36 AM
 * Author: Ranul Deepanayake
 * Contains custom string manipulation functions aimed at reducing code space in an embedded environment.
 */ 


#ifndef STRINGFUNCTIONS_H_
#define STRINGFUNCTIONS_H_

#include <string.h>

/*
Convert a number to a char array. The source array must be SIZE+ 1 long to accommodate the null character.
Can handle negative numbers. Floating point numbers will be rounded off to integers (both positive and negative)
*/
void sprintf2(int number, char string[]);



#endif /* STRINGFUNCTIONS_H_ */