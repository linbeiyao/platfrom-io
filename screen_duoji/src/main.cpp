#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Servo.h>
#include "libc.h"

//设置舵机最小值、最大值
#define PAUSE_MIN 500
#define PAUSE_MAX 2500

//定义舵机针脚
#define duoji_1 15
#define duoji_2 4
#define duoji_3 5
#define duoji_4 18

//定义屏幕针脚
U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI oled(U8G2_R0,13,12,26,27,14);

Servo my_servo1;
Servo my_servo2;
Servo my_servo3;
Servo my_servo4;



void init();




void setup() 
{
  //初始化舵机
  //分配硬件定时器
  ESP32PWM::allocateTimer(0);
  //设置频率
  my_servo1.setPeriodHertz(50);
  my_servo2.setPeriodHertz(50);  
  my_servo3.setPeriodHertz(50);
  my_servo4.setPeriodHertz(50); 
  //关联 Servo 对象
  my_servo1.attach(duoji_1,PAUSE_MIN,PAUSE_MAX);
  my_servo2.attach(duoji_2,PAUSE_MIN,PAUSE_MAX);  
  my_servo3.attach(duoji_3,PAUSE_MIN,PAUSE_MAX);
  my_servo4.attach(duoji_4,PAUSE_MIN,PAUSE_MAX);  
  
  //测试舵机
  //init();

  //初始化屏幕
  //测试屏幕
  oled.begin();
  oled.enableUTF8Print();
  oled.home();
  oled.print("初始化完成！！");

}

void loop() {
  oled.print("初始化完成！！");
}






void init()                 //舵机测试函数
{
  my_servo1.write(0);
  delay(500);
  my_servo2.write(0);
  delay(500);
  my_servo3.write(0);
  delay(500);
  my_servo4.write(0);
  delay(500);
  my_servo1.write(180);
  delay(500);
  my_servo2.write(180);
  delay(500);
  my_servo3.write(180);
  delay(500);
  my_servo4.write(180);
  delay(500);
}


