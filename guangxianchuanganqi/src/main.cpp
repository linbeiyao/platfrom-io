#include <Arduino.h>

#define guang_pin 15
          
void setup() {
  Serial.begin(115200);
  pinMode(guang_pin,OUTPUT);
}

void loop() {
//在窗口调试窗口输出光线强度
Serial.println(analogRead(guang_pin));
delay(1000);

}

