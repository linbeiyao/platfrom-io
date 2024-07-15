#include <Arduino.h>
//#include <lvgl.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>

void TFT_init(){
  tft.init();              
  tft.setRotation(4);                 //初始化
  Serial.println("TFT_INIT_OK");  
}
/******************************************************************************/
void drawAr(){
  
  clk.setColorDepth(8);
  clk.createSprite(64, 64);//创建窗口
  clk.fillSprite(0x0000);   //填充率
  clk.setTextDatum(CC_DATUM);   //设置文本数据
  clk.setTextColor(TFT_WHITE, bgColor);
  clk.setSwapBytes(true);
  clk.pushImage(0, 0,  64, 64, Astronaut[i]); 
  clk.println("ok");
  clk.pushSprite(64,64);  //窗口位置
  clk.deleteSprite();// (10,55) 显示 64 × 64 像素的图片
  if(millis() - imgtime>150){	
    imgtime = millis();		//延时
    i+=1;								//下一帧
    if(i>8){i=0;}	
}
}
/**********************************************************************************************/
void setup() {
  Serial.begin(115200);
 TFT_init();

  Serial.println("OKKKK");
   tft.fillScreen(TFT_BLACK);
   scanNetworks();						//扫描wifi并打印信息
   connect();							//连接到指定wifi
   timeClient.begin();                 //连接NTP服务器
   timeClient.setTimeOffset(28800);    //时区偏移
   wea.Get();
//    Serial.println(land.post(IPdata,land.url_land));
//    delay(5000);
//    land.post(IPdata,land.url_unland);
   //imu.init();
   
                   //获取天气
}

void loop() {
  clk.loadFont(MY_FONT_Z);
  clk.setColorDepth(8);
  clk.createSprite(128, 128);//创建窗口
  clk.fillSprite(0x0000);   //填充率
  clk.setTextDatum(CC_DATUM);   //设置文本数据
  clk.setTextColor(TFT_WHITE, bgColor); 
  clk.setCursor(20,20,2);
  clk.setSwapBytes(true);
  //Serial.println(timeClient.getFormattedTime());
  clk.println(timeClient.getFormattedTime());
  clk.println(wea.now_address);
  clk.println(wea.now_weather);  
  clk.println(wea.now_temperature+"℃");
  clk.pushImage(60, 55,  64, 64, Astronaut[i]); // (10,55) 显示 64 × 64 像素的图片
  clk.pushSprite(0,0);  //窗口位置
  clk.deleteSprite();
  clk.unloadFont();
    if(millis() - imgtime>150){	
    imgtime = millis();		//延时
    i+=1;								//下一帧
    if(i>8){i=0;}	
    }
