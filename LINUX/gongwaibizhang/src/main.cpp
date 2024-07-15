#include <Arduino.h>
#define ledPin  23
#define jcPin 22



void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(jcPin,INPUT);
  digitalWrite(ledPin,LOW);
  
}

void loop() {
  
  if(digitalRead(jcPin) == LOW)
  {
   digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }

}

