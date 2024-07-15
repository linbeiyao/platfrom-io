#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGH 64
#define OLED_MOSI 13
#define OLED_CLK 18
#define OLED_DC 2
#define OLED_CS 4
#define OLED_RESET 15

Adafruit_SSD1306 oled(SCREEN_WIDTH,SCREEN_HEIGH,OLED_MOSI,OLED_CLK,OLED_DC,OLED_RESET,OLED_CS);

//初始化进度条
int progress = 0;



void setup() {
  oled.begin();
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);
  oled.println("wancheng。。。。");
  oled.display();
  delay(5000);
}

void loop() {

  oled.clearDisplay();

  oled.setCursor(25,40);
  oled.println("Process");

 //显示进度条边框
  oled.drawRoundRect(0,10,128,20,5,SSD1306_WHITE);
  //显示进度
  oled.fillRoundRect(5,15,progress,10,2,SSD1306_WHITE);

  if (progress < 118)
  {
    progress++;
  }
  else
  {
    progress = 0;
  }

  oled.display();
  delay(500);


}

