/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


//Beginning of Auto generated function prototypes by Atmel Studio
int readSerial(char* result);
//End of Auto generated function prototypes by Atmel Studio

//Extracted from the GSM library.
char message[100];
String s; 

void setup() {
  Serial.begin(9600);
  while(!Serial){}//Wait till serial connection is established.
}

void loop() {
  s= "";
  readSerial(message);
  Serial.println(message);
  Serial.println(s);
}

int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      result[i] = inChar;
      //s.concat(result[i]); //Additional.
        i++;
    }
  }
}
