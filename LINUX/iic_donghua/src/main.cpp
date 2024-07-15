#include <Arduino.h>  
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

//屏幕的高度、宽度
#define SCREEN_height 64
#define SCREEN_width 128

void setup() {

  Adafruit_SSD1306 oled = Adafruit_SSD1306(SCREEN_width, SCREEN_height, &Wire);
  oled.begin(SSD1306_SWITCHCAPVCC,0x3c);
  oled.clearDisplay();
  oled.setTextColor(SSD1306_WHITE);
  oled.setCursor(0,0);
  oled.setTextSize(1);
  oled.println("SYSTEM INIT successed!!");
  oled.println("^_^");
  oled.drawBitmap()
  oled.display();

}

void loop() {

}


