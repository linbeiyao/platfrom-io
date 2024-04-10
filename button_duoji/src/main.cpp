#include <Arduino.h>
#include <U8g2lib.h>
#include <ESP32Servo.h>

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI oled(U8G2_R0,13,12,26,27,14);


//设置舵机最小值、最大值
#define PAUSE_MIN 500
#define PAUSE_MAX 2500

Servo my_servo1;
Servo my_servo2;
Servo my_servo3;
Servo my_servo4;

//定义舵机针脚
#define duoji_1 15
#define duoji_2 4
#define duoji_3 5
#define duoji_4 18
//定义按钮针脚
#define button1_pin 19
#define button2_pin 21
#define button3_pin 22
#define button4_pin 23

void setup()
{
  //舵机
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
  //初始化舵机
  my_servo1.write(0);
  my_servo2.write(0);
  my_servo3.write(0);
  my_servo4.write(0);


  //按钮
  //设置按钮针脚模式 
  pinMode(button1_pin,INPUT_PULLDOWN);
  pinMode(button2_pin,INPUT_PULLDOWN);
  pinMode(button3_pin,INPUT_PULLDOWN);
  pinMode(button4_pin,INPUT_PULLDOWN);
  
  //屏幕
  oled.begin();
  oled.enableUTF8Print();
  //设置字体
  oled.setFont(u8g2_font_unifont_t_chinese2);
  //设置字体方向
  oled.setFontDirection(0);

  
}



int pause1_width = 0;
int pause2_width = 0;
int pause3_width = 0;
int pause4_width = 0;




void turnduoji(Servo  current_servo,int *width);

void Turn_repeatedly(Servo & servo);

void loop()
{
  // if ( digitalRead(button1_pin) == HIGH )
  // { 
  //   turnduoji(my_servo1,&pause1_width);
  //   delay(1000);
  //   Serial.println("舵机一\n");

  // }
  //   if ( digitalRead(button2_pin) == HIGH )
  // { 
  //   turnduoji(my_servo2,&pause2_width);
  //   delay(1000);
  //   Serial.println("舵机二\n");
  // }
  //   if ( digitalRead(button3_pin) == HIGH )
  // { 
  //   turnduoji(my_servo3,&pause3_width);
  //   delay(1000);
  //   Serial.println("舵机三\n");
  // }
  //   if ( digitalRead(button4_pin) == HIGH )
  // { 
  //   turnduoji(my_servo4,&pause4_width);
  //   delay(1000);
  //   Serial.println("舵机四\n");
  // }
  // digitalWrite(button1_pin,LOW);
  // digitalWrite(button2_pin,LOW);
  // digitalWrite(button3_pin,LOW);
  // digitalWrite(button4_pin,LOW);

  
  if ( digitalRead(button1_pin) == HIGH )
  { 
    //Serial.println("舵机一\n");
    //从0转到180,再从180转动到0
    Turn_repeatedly(my_servo1);
    
    
  }
    if ( digitalRead(button2_pin) == HIGH )
  { 
    //Serial.println("舵机二\n");
    //从0转到180,再从180转动到0
    Turn_repeatedly(my_servo2);

    
    
  }
    if ( digitalRead(button3_pin) == HIGH )
  { 
    //Serial.println("舵机三\n");
    //从0转到180,再从180转动到0
    Turn_repeatedly(my_servo3);
    
  }
    if ( digitalRead(button4_pin) == HIGH )
  { 
    //Serial.println("舵机四\n");
    //Serial.println("4");
    //从0转到180,再从180转动到0
    //Turn_repeatedly(my_servo4);

    my_servo4.write(180);
    delay(1000);
    my_servo4.write(0);  

   
    
  }






}


void Turn_repeatedly(Servo & servo)
{
  servo.write(180);
  delay(1000);
  servo.write(0);

  oled.clearBuffer();
  oled.setCursor(0, 15);
  oled.print("Hello GeeksMan!");

}


void turnduoji(Servo current_servo,int *width)
{
  if(*width > 180)
    *width = 0;
  current_servo.write(*width);
  
}
