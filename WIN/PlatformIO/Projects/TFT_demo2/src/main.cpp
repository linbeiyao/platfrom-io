#include <Arduino.h>
//#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include "OneButton.h"

TFT_eSPI tft = TFT_eSPI();
OneButton k1(18,true);



void k1Click(); // 按钮单击时间
void k1DoubleClick(); //按钮双击事件


void setup(){
  Serial.begin(115200);
  Serial.println("Serial.begin()");
  
  tft.init();
  tft.setRotation(2);           // 参数 0 1 2 3 分别表示 0度 90度 180度 270度
                                // 设置为 2 正好从针脚向下 右侧是按钮 
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(10,10,1);       // 设置起始坐标（10，10）,1号字体
  tft.setTextColor(TFT_YELLOW); // 设置文本颜色为黄色
  tft.setTextSize(1);           // 设置文字大小为2号（1-7）
  tft.println("Hello,TFT!!");
  tft.setTextSize(2);

  tft.fillCircle(30,30,10,TFT_BLUE);        // 画圆
  tft.drawLine(10,50,118,50,TFT_WHITE);     // 画线
  tft.drawPixel(100,100,TFT_RED);             // 画点

  tft.setTextColor(TFT_YELLOW,TFT_BLUE);
  tft.setCursor(10,80,1);
  tft.println("wangyongcheng");



  // tft.fillScreen(TFT_WHITE);
  // tft.drawArc(60,60,20,10,10,100,TFT_RED,TFT_BLUE);
  

  k1.setDebounceMs(80);   // 设置稳定之前的 MS 数
  k1.setClickMs(400);      // 设置单击按钮的时间长度
  k1.setPressMs(1000);    // 设置长按按钮的时间长度
  
  
  k1.reset();//清除一下按钮状态机的状态
  k1.attachClick(k1Click);  // 附加k1的单击事件
  k1.attachDoubleClick(k1DoubleClick);
  
  

  tft.setCursor(0,0);
  tft.setTextColor(TFT_WHITE,TFT_RED);
  tft.setTextSize(1);

  pinMode(13,INPUT);


}

//按钮检测状态子程序
void button_attach_loop(){
    //不断检测按钮按下状态
    k1.tick();
}




void loop(){
button_attach_loop();
Serial.println(analogRead(13));
 

}



void k1Click(){ // k1 click
  Serial.println("k1 clicked");
  tft.println("k1 clicked");
}
void k1DoubleClick(){ //k1 double click
  tft.println("k1 DoubleClicked");
}