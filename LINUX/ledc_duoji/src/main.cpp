#include <Arduino.h>
#include<ESP32Servo.h>

// 1/20 秒，50Hz 的频率，20ms 的周期，这个变量用来存储时钟基准。
#define FREQ 50
// 通道(高速通道（0 ~ 7）由 80MHz 时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。) 
#define CHANNEL 0
//分辨率设置为 8，就是 2 的 8 次方，用 256 的数值来映射角度
#define RESOLUTION 8
//定义舵机 PWM 控制引脚
#define SERVO 2

//定义函数用来输出 pwm 的占空比
int calcuatePWM(int degree)
{
  //20ms 周期内，高电平持续时长 0.5 - 2.5 ms，对应 0-180 度舵机角度
  //对应 2.5 ms （0.5ms / (20ms/256)）
  float min_width = 0.6 / 20 * pow(2,RESOLUTION);
  //对应 2.5 ms  (2.5ms / (20ms/256))
  float max_width = 2.5 / 20 * pow(2,RESOLUTION);

  if (degree < 0)
  {
    degree = 0;
  }
  else
  {
    degree = 180;
  }

  //返回度数对应的高电平的数值
  return (int)(((max_width - min_width) / 180) * degree + min_width);  

}

void setup() {

  //用于设置 LEDC 通道的频率和分辨率
  ledcSetup(CHANNEL,FREQ,RESOLUTION);
  //将通道与对应的引脚连接
  ledcAttachPin(SERVO,CHANNEL);
}

void loop() {

for (int i = 0; i <= 180; i += 180)
{
  //输出pwm，设置 ledc 通道的占空比
  ledcWrite(CHANNEL,calcuatePWM(i));
  delay(1000);
}




}