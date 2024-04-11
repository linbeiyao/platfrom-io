#include <Arduino.h>
#include <ESP32Servo.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_GFX.h>
#include <Wire.h>
#include "libc.h"
#include <U8g2lib.h>
#include "esp_task_wdt.h"
#include "net_now.h"
//#include "bule.h"





////////////////////////////////////////////////////////////////////////////////////////////////////////////

//实现位图
void display_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_SW_I2C &oled,bool &flag);
//检测asr连接针脚状态，给对应舵机针脚输出 pwm 信号
void asr_servo(int pin1,int pin2,int pin3,int pin4,Servo &servo_1,Servo &servo_2,Servo &servo_3,Servo &servo_4,U8G2_SSD1306_128X64_NONAME_F_SW_I2C &oled);
//舵机转动函数
void TURN(Servo &servo, bool &turning);
// 语音模块
void ASR_RRO();

//多线程 语音检测
void taskCore0(void *pvParameters);
//多线程 热敏电阻
void taskCore1(void *pvParameters);

//初始化屏幕
void init_oled();

//beeper初始化
void beeper_start();


//光线传感 初始化函数 和 检测函数
void guanxian_init();
void guangxian();


///////////////////////////////////////////////////////////////////////////////////////////////////////////

//led_pin
#define led_pin 32

//光线传感器 针脚
#define guang_d0_pin 34
#define guang_a0_pin 35

//强光标识
     bool qiang_flag = false;
     bool qiang_xianshi_flag;

//屏幕的高度、宽度
#define SCREEN_height 64
#define SCREEN_width 128

bool main_UI_flag = false;

//实例化屏幕
//Adafruit_SSD1306 oled = Adafruit_SSD1306(SCREEN_width,SCREEN_height,&Wire);
 U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0,22,21,U8X8_PIN_NONE);

 

//舵机 start ==============================================
//舵机的针脚
#define servo_1_pin 13
#define servo_2_pin 12
#define servo_3_pin 14
#define servo_4_pin 27

//asr pro pin
#define asr_1_pin 15
#define asr_2_pin 2
#define asr_3_pin 4
#define asr_4_pin 5
#define asr_output_pin 18

//舵机最大转动角度
#define MAX_WIDTH 2500
#define MIN_WIDTH 500

Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_4;


//舵机的运动状态
bool servo1_turning = false;
bool servo2_turning = false;
bool servo3_turning = false;
bool servo4_turning = false;


//舵机 end ================================================

//蜂鸣器
#define beep_pin 26


// 红外传感器 PIN
#define IR_pin 19

//热敏电阻 PIN 
#define ptc_pin 23












void setup() {


  Serial.begin(115200);
  Serial.println(" ฅ^•ﻌ•^ฅ 串口监视器准备好了吗！？！ ");
  delay(1000);
  Serial.println("我要开始初始化喽......");
  delay(500);




//wifi start ////////////////////////////////////////////

Serial.println("开始初始化wifi");
wifi_init();



//NOW start ==============================================

  now_init();





//舵机 start ==============================================
  Serial.println("嘿嘿! 舵机初始化,舵机初始化！！⁄(⁄ ⁄•⁄ω⁄•⁄ ⁄)⁄ ");
  delay(500);
  //分配硬件定时器
  ESP32PWM::allocateTimer(0);
  //设置频率
  servo_1.setPeriodHertz(50);  
  servo_2.setPeriodHertz(50);
  servo_3.setPeriodHertz(50);
  servo_4.setPeriodHertz(50);

  //关联 servo 对象与 GPIO 引脚(asr_servo，设置脉宽范围

  servo_1.attach(servo_1_pin,MIN_WIDTH,MAX_WIDTH);  
  servo_2.attach(servo_2_pin,MIN_WIDTH,MAX_WIDTH);
  servo_3.attach(servo_3_pin,MIN_WIDTH,MAX_WIDTH);
  servo_4.attach(servo_4_pin,MIN_WIDTH,MAX_WIDTH); 
  
 
  



  Serial.println("舵机初始化完成喽！！");
  delay(3000);
//舵机 end ================================================



  init_oled();


  // // beeper start //////////////////////////////////////////
    beeper_start();

  // 红外传感器 start ///////////////////////////////////////
  Serial.println("哈喽哈喽，这里是红外传感器！！！");
  Serial.println("红外传感器要开始测试喽......");
  delay(500);
  pinMode(IR_pin,INPUT);
  bool flag = false;
  digitalWrite(IR_pin,HIGH);

  while(true){digitalWrite(beep_pin,HIGH);
    Serial.println(int(digitalRead(IR_pin)));
    if (digitalRead(IR_pin) == 0 && flag == false)
    {
      Serial.println("肯定有人在红外传感器前！！对吧？！");
      delay(500);
      flag = true;
      
      break;
    }
    else
    {
      Serial.println("看来你不在红外传感器前......");
      delay(500);
    }
    delay(1000);
  }
  Serial.println("红外传感器测试结束了!! 欧耶!!");
  delay(500);




  // 语音模块
  void ASR_RRO();



// 热敏电阻模块 start
  pinMode(ptc_pin,INPUT);



  

  // 光线传感 初始化
  guanxian_init();
   


  //vTaskStartScheduler();
  oled.clearDisplay();
  UI_main(oled,main_UI_flag);

  Serial.println("初始化已经完成喽, 开始操作吧！ ^_^ ");



  xTaskCreate(taskCore0, "asr_servo", 8888, NULL, 1, NULL);
  xTaskCreate(taskCore1, "ptc", 4028, NULL, 1, NULL);


  //创建线程任务 检测传输过来的 NOW 数据 如果有异常作出对应响应
  void NOWDATA_test(void *pvParameters);
  //xTaskCreate(NOWDATA_test,"NOWDATA_TEST",4086,NULL,1,NULL);  //应该在接收到数据的回调函数中创建线程任务


}



char beep_flag = 0;



void loop() {

  guangxian();

  // WiFi.mode(WIFI_AP_STA);
  // Serial.println(WiFi.macAddress());

  // //测试图像
  //   display_bmp(0,0,64,64,chuyu_64,oled);
  //   display_bmp(0,0,64,64,kehuishouwu_64,oled);
  //   display_bmp(0,0,64,64,qita_64,oled);
  //   display_bmp(0,0,64,64,youhai_64,oled);
  //   display_bmp(0,0,128,64,chuyu_128,oled);    
  //   display_bmp(0,0,128,64,kehuishouwu_128,oled);
  //   display_bmp(0,0,128,64,qita_128,oled);
  //   display_bmp(0,0,128,64,youhai_128,oled);

  //蓝牙测试
  //test_data();

  delay(500);


}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////











// 热敏电阻模块 //////////////////////////////////////////////
void ptc()
{
  //为低电压就代表高温度，发出报警,发出报警只需要给对应针脚高电压即可
  if(!digitalRead(ptc_pin))
  {
    Serial.println(digitalRead(ptc_pin));
    Serial.println("///////////////////////////");
    Serial.println("警告：垃圾桶内可能着火！！");
    Serial.println("警告：垃圾桶内可能着火！！");
    Serial.println("警告：垃圾桶内可能着火！！");
    Serial.println("///////////////////////////");    
    display_bmp(65,0,64,64,jinggao_64_64,oled);



    digitalWrite(beep_pin,LOW);
  }
  else
  {
    digitalWrite(beep_pin,HIGH);
  }
}
// 热敏电阻模块 //////////////////////////////////////////////




void beeper_start()
{
  pinMode(beep_pin,OUTPUT);
  Serial.println("Beeper rell!!");

}

void taskCore0(void *pvParameters) {
 while (1) {

        esp_task_wdt_reset();
        // 检查舵机状态并进行转动
        if (!servo1_turning && digitalRead(asr_1_pin)) {
            oled.clearDisplay();

            Serial.println("舵机1开始转!");
            display_bmp(0,0,128,64,chuyu_128,oled,main_UI_flag);
            TURN(servo_1, servo1_turning);
            oled.clearDisplay();
            //clear_workspace(oled);
            UI_main(oled,main_UI_flag);
            delay(1000);
        }
        if (!servo2_turning && digitalRead(asr_2_pin)) {
            oled.clearDisplay();
            Serial.println("舵机2开始转!");
            display_bmp(0,0,128,64,youhai_128,oled,main_UI_flag);
            TURN(servo_2, servo2_turning);
            oled.clearDisplay();
            //clear_workspace(oled);
            UI_main(oled,main_UI_flag);
            delay(1000);
        }
        if (!servo3_turning && digitalRead(asr_3_pin)) {
            oled.clearDisplay();
            Serial.println("舵机3开始转!");
            display_bmp(0,0,128,64,kehuishouwu_128,oled,main_UI_flag);
            TURN(servo_3, servo3_turning);
            oled.clearDisplay();
            //clear_workspace(oled);
            UI_main(oled,main_UI_flag);
            delay(1000);
        }
        // Serial.println(servo4_turning);
        // Serial.println();
        // Serial.println(digitalRead(asr_1_pin));
        // Serial.println(digitalRead(asr_2_pin));
        // Serial.println(digitalRead(asr_3_pin));
        // Serial.println(digitalRead(asr_4_pin));
        if (!servo4_turning && digitalRead(asr_4_pin)) {
            oled.clearDisplay();
            Serial.println("舵机4开始转!");
            display_bmp(0,0,128,64,qita_128,oled,main_UI_flag);
            TURN(servo_4, servo4_turning);
            oled.clearDisplay();
            //clear_workspace(oled);
            UI_main(oled,main_UI_flag);
            delay(1000);
        }

        // 添加适当的延迟，以防止任务占用 CPU 过多
        vTaskDelay(pdMS_TO_TICKS(100)); // 例如，延迟 100 毫秒
        // vTaskDelay(1000 / portTICK_PERIOD_MS);

 }
}

void taskCore1(void *pvParameters) {
 while (1) {

        esp_task_wdt_reset();
        vTaskDelay(1000);
        ptc();
        vTaskDelay(1000);

 }
}

//舵机转动函数
 void TURN(Servo &servo, bool &turning) {
    // 将转动状态置为 true
    turning = true;

    // 舵机转动到0度
    servo.write(0);
    delay(3000);

    // 舵机转动到180度
    servo.write(180);
    delay(1000);

    // 将转动状态置为 false
    // display_bmp(0,0,128,64,chuyu_128);
    turning = false;
}

//光敏电阻
  //初始化函数
  void guanxian_init()
  {

    pinMode(guang_a0_pin,INPUT);
    pinMode(guang_d0_pin,INPUT);
    pinMode(led_pin,OUTPUT);
  }
  //感应函数
  void guangxian()
  {
    delay(100);
    Serial.println("光线 D0 A0");
    Serial.println(digitalRead(guang_d0_pin));
    Serial.println(analogRead(guang_a0_pin));


    //读取模拟输入值
    int sensorValue = analogRead(guang_a0_pin);
    sensorValue = sensorValue - 3200;

    
    
    if (sensorValue < 0)
    {
      sensorValue = 0;
      qiang_flag = true;
      analogWrite(led_pin,sensorValue);
    }
    else
    {
      qiang_flag = false;
    }


    if (qiang_flag && !qiang_xianshi_flag)
    {
      qiang_xianshi_flag = true;
      clear_workspace(oled);
      Serial.println("光照强度已经达到预设值，关闭 led 灯照！");
      oled.setCursor(10,32);
      oled.setFont(u8g2_font_wqy12_t_chinese2);
      oled.println("光照强度已经达到预设值");
      oled.setCursor(10,45);
      oled.println("关闭 led 灯照！");
      oled.sendBuffer();
      analogWrite(led_pin,sensorValue );
      delay(1000);

      UI_main(oled,main_UI_flag);//在屏幕上输出强光 提示后 恢复主页面
    }
    else if (qiang_flag && qiang_xianshi_flag || !qiang_flag )
    {
      if (main_UI_flag)
      {
        UI_main(oled,main_UI_flag);
      }
      
      
      if (!qiang_flag)
      {
        qiang_xianshi_flag = false;
      }
      
    }
      analogWrite(led_pin,sensorValue );
  }

// 语音模块
void ASR_RRO()

{

  pinMode(asr_1_pin,INPUT);  
  pinMode(asr_2_pin,INPUT);
  pinMode(asr_3_pin,INPUT);
  pinMode(asr_4_pin,INPUT);  
  pinMode(asr_output_pin,OUTPUT);

}

//初始化屏幕
void init_oled()
{
  Serial.println("开始初始化屏幕！！");
  int i = 0;
  int j = 128;
  oled.begin();
  
  oled.setFont(u8g2_font_unifont_t_chinese3);
  oled.enableUTF8Print();
  oled.setFontDirection(0); // 设置文本方向


  // oled.setCursor(0, 16);
  // oled.print("u8g2初始化完成!!");
  // oled.drawLine(0,18,128,18);
  // oled.setFont(u8g2_font_wqy12_t_chinese3);
  // oled.setCursor(0,32);
  // oled.println("触发人体感应进行使用....");
  // oled.sendBuffer();

  // while(i != 128)
  // {
  //   oled.drawBox(0,34,i++,10);
  //   oled.sendBuffer();
  // }
  
  oled.firstPage();
  oled.setFont(u8g_font_6x10r);
  do{
    oled.drawRBox(25,25,i++,10,0.001);
    oled.drawStr(40,25+10+12,"LOADING.....");
    
  }while(i != 85 && !oled.nextPage());

    oled.clearDisplay();
    oled.setFont(u8g2_font_unifont_t_chinese3);
    oled.setCursor(0, 16);
    oled.print("u8g2初始化完成!!");
    oled.drawLine(0,18,128,18);
    oled.setFont(u8g2_font_wqy12_t_chinese3);
    oled.setCursor(0,32);
    oled.println("触发人体感应进行使用...."); 
    oled.sendBuffer();



  oled.setCursor(0,51);
  oled.setFont(u8g2_font_unifont_t_chinese3); 
 
  Serial.println("屏幕初始化结束！！");
}


//创建线程任务 检测传输过来的 NOW 数据 如果有异常作出对应响应
  void NOWDATA_test(void *pvParameters){

    while (true)
    {
      Serial.println("多线程 now 接受数据");

      Serial.println(MY_bin.BIN1);
      Serial.println(MY_bin.BIN2);
      Serial.println(MY_bin.BIN3);
      Serial.println(MY_bin.BIN4);
      Serial.println(MY_bin.flag);
      MY_bin.flag = 99;
      if (MY_bin.flag == 99)
      {
        Serial.println("这是个无用数据");
      }
      else
      {
            


          if (MY_bin.flag == yichu_flag)
          {

            Serial.println("垃圾桶溢出传感器的状态：");
            if (!MY_bin.BIN1)  // 传输过来 溢出为 0 取反为真
            {
                Serial.println("厨余垃圾桶溢满!!");
                display_part_bmp(0,0,64,64,chuyu_64,oled);
                display_part_bmp(64,0,64,64,yichu_64_64,oled);
                vTaskDelay(1000);
                
                UI_main(oled,main_UI_flag);
            }
            else
            {
                Serial.println("厨余垃圾桶暂未溢满!!");
            }
            
            if (!MY_bin.BIN2)
            {
                Serial.println("可回收物垃圾桶溢满!!");
                display_part_bmp(0,0,64,64,kehuishouwu_64,oled);
                display_part_bmp(64,0,64,64,yichu_64_64,oled);
                vTaskDelay(1000);
                
                UI_main(oled,main_UI_flag);
            }
            else
            {
                Serial.println("可回收物垃圾桶暂未溢满!!");
            }
            
            if (!MY_bin.BIN3)
            {
                Serial.println("有害垃圾桶溢满!!");
                display_part_bmp(0,0,64,64,youhai_64,oled);
                display_part_bmp(64,0,64,64,yichu_64_64,oled);
                vTaskDelay(1000);
                UI_main(oled,main_UI_flag);
            }
            else
            {
                Serial.println("有害垃圾桶暂未溢满!!");
            }
            
            if (!MY_bin.BIN4)
            {
                Serial.println("其他垃圾桶溢满!!");
                display_part_bmp(0,0,64,64,qita_64,oled);
                display_part_bmp(64,0,64,64,yichu_64_64,oled);
                vTaskDelay(1000);
                UI_main(oled,main_UI_flag);
            }
            else
            {
                Serial.println("其他垃圾桶暂未溢满!!");
            }
            delay(1000);
          }
        
        if (MY_bin.flag == zhaohuo_flag)
        {
            Serial.println("垃圾桶温度传感器的状态：");
            if (MY_bin.BIN1)
            {
                Serial.println("厨余垃圾桶内温度异常,可能着火，请及时检查!!");
            }
            else
            {
                Serial.println("厨余垃圾桶温度正常!!");
            }
            
            if (MY_bin.BIN2)
            {
                Serial.println("可回收物垃圾桶内温度异常,可能着火，请及时检查!!");
            }
            else
            {
                Serial.println("可回收物垃圾桶温度正常!!");
            }
            
            if (MY_bin.BIN3)
            {
                Serial.println("有害垃圾桶内温度异常,可能着火，请及时检查!!");
            }
            else
            {
                Serial.println("有害垃圾桶温度正常!!");
            }
            
            if (MY_bin.BIN4)
            {
                Serial.println("其他垃圾桶内温度异常,可能着火，请及时检查!!");
            }
            else
            {
                Serial.println("其他垃圾桶温度正常!!");
            }
            delay(1000);
        }
        if (MY_bin.flag == 99)
        {
          Serial.println("无用的 NOW 数据");
          MY_bin.flag = 99;
        }
        



    

      }

      vTaskDelay(1000);

        
    
    }
    



  }



