#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>

// 定义 OLED屏幕的分辨率
Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

void setup()
{
  Serial.begin(9600);

  Serial.println("OLED test");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay(); // 清空屏幕

  display.setTextSize(1);              // 设置字体大小
  display.setTextColor(SSD1306_WHITE); // 设置字体颜色
  display.setCursor(0, 0);             // 设置光标位置
  display.println("Hello,world!!");    // 输出字符
  display.println("   by huangjiaoyuan");
  display.display(); // 显示
  display.println("OlED successed!!");
  display.display();
  delay(5000);
  display.clearDisplay();
  // display.clearDisplay();
  // display.setCursor(0,0);
  // display.println("ESP32 - WROOM - 32");

  // display.display();


  //display.drawLine(0,0 , 128, 0, SSD1306_WHITE);
  //display.drawLine(0,0 , 128, 1, SSD1306_WHITE);
  display.display();
}

int x = 0;
int y = 0;
int x2 = 128, y2 = 0;

void loop()
{
  x2 = 128, y2 = 0;

  // display.println("ESP32 - WROOM - 32");
  // display.display();
  while (y2 <= 64)
  {
    display.drawLine(x, y, x2, y2, SSD1306_WHITE);
    y2 += 4;
    display.display();
    delay(50);
    Serial.println(y2);
  }
  while (x2 >= 0)
  {
    display.drawLine(x, y, x2, y2, SSD1306_WHITE);
    x2 -= 8;
    display.display();
    delay(50);
    Serial.println(x2);
  }
  
  display.clearDisplay();
  display.fillScreen(SSD1306_WHITE);
  display.display();
  delay(500);
  display.fillScreen(SSD1306_BLACK);
  delay(500);
  display.display();
  
}
