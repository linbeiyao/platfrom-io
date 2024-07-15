#include <Arduino.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI oled(U8G2_R0,18,13,4,2,15);
//U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI oled(U8G2_R0,13,12,26,27,14);

void setup() {
  
  oled.begin();
  oled.enableUTF8Print();
  oled.setFont(u8g2_font_unifont_t_symbols);
}

void loop() {
  //设置字体
  oled.setFont(u8g2_font_unifont_t_chinese2);
  //设置字体方向
  oled.setFontDirection(0);
  //
  oled.clearBuffer();
  oled.setCursor(0, 15);
  oled.print("Hello GeeksMan!");
  oled.setCursor(0, 40);
  oled.print("你好, U8g2!");
  oled.setFont(u8g2_font_unifont_t_symbols);
  oled.drawUTF8(0, 55, "Snowman: ☃");
  oled.sendBuffer();
 
}

