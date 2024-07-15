#include <Arduino.h>
//#include <Adafruit_GFX.h>
#include <Wire.h>
//#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>
#include <ESP32Servo.h>
#include "BIN_logo.h"
#include "OLEd.h"

typedef U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
bool flag;



//实现位图
void display_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled,bool &flag);
void display_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled);
void display_part_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled);

// // IR start /////////////////////////////////////////////
//     //检测红外传感器的状态
//     inline int IR_Sensitive(int IR_pin){
//         bool flag = false;
//         digitalWrite(IR_pin,HIGH);
        
//         Serial.println(int(digitalRead(IR_pin)));
//         if (digitalRead(IR_pin) == 0 && flag == false)
//         {
//         Serial.println("肯定有人在红外传感器前！！对吧？！");
//         delay(500);
//         flag = true;
//         return HIGH;
//         }
//         else
//         {
//         Serial.println("看来你不在红外传感器前......");
//         return LOW;
//         delay(500);
//         }
        
//         Serial.println("红外传感器测试结束了!! 欧耶!!");
//     }
// // IR  end ////////////////////////////////////////////

// beeper start ======================================
    //蜂鸣器发音函数 
    //参数：次数，低电压时长（单位：s），高电压时长（单位：s）
    void bell(int pin,int count,float s1,float s2)
    {
        
       for(int i = 1 ; i <= count ; i++)
       {
            digitalWrite(pin,LOW);
            Serial.print("第 ");
            Serial.print(i);
            Serial.println(" 次蜂鸣！");
            delay(s1 * 1000);
            digitalWrite(pin,HIGH);
            delay(s2 * 1000);
            
       }
       digitalWrite(pin,PULLDOWN);
    }



// beeper end ========================================



// 舵机 start /////////////////////////////////////////
    // 自定义转动角度，转动时长函数
    void TURN(Servo & servo,int angle1,int angle2,int s)
    {
        Serial.println("//////// 舵机转动函数 start ////////////");
        Serial.print("舵机首先转动到 "); Serial.print(angle1); Serial.println("度");
        Serial.print("舵机首先转动到 "); Serial.print(angle1); Serial.println("度");
        Serial.print("每次转动之后等待 ");Serial.print(s);Serial.println(" 秒");
        Serial.println("//////// 舵机转动函数 end //////////////");

        servo.write(angle1);
        delay(s * 1000);
        servo.write(angle2);
        delay(s * 1000);
        
    }  
    // 快速转动函数
     void TURN(Servo & servo)
    {
        servo.write(0);
        delay(3000);
        servo.write(180);
        delay(1000);
        
    } 





   
// 舵机 end ///////////////////////////////////////////



// OLED start /////////////////////////////////////////
void UI_main(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled,bool &flag)
{
    clear_workspace(oled);
    display_part_bmp(0,17,128,47,sizhongleibie_128_47,oled);
}



// OLED end ///////////////////////////////////////////




// oled start /////////////////////////////////////////////
    //显示 bitmap 图
    void display_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled,bool &flag )
    {
      oled.clearDisplay();
      oled.clearBuffer();
      oled.drawXBMP(x,y,w,h,bitmap);
      oled.sendBuffer();
      flag = false;

    }
    void display_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled)
    {
      oled.clearDisplay();
      oled.clearBuffer();
      oled.drawXBMP(x,y,w,h,bitmap);
      oled.sendBuffer();

    }
    void display_part_bmp(int x,int y,int w,int h, const uint8_t *bitmap,U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled)
    {
     
      oled.drawXBMP(x,y,w,h,bitmap);
      oled.sendBuffer();

    }
// oled end ///////////////////////////////////////////////