#include <Arduino.h>


void setup() {
  Serial.begin(9600);
  Serial.println("小北，在初始化啦！！^__^");
  pinMode(2,INPUT);
}

void loop() {

if (digitalRead(2) == HIGH)
{
  Serial.println("我是小助手，小北！！！");
}
}



