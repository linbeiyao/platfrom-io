#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Servo.h>


void TURN(Servo & servo)
{
    servo.write(0);
    delay(1000);
    servo.write(180);
    delay(1000);
}