
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <U8g2lib.h>

#define TITLE_LINE 13
#define TOP 15
#define workspace_HIGH 48
#define workspace_WIDTH 128




//清空工作区的函数
void clear_workspace(U8G2_SSD1306_128X64_NONAME_F_HW_I2C &oled) {
    oled.clearDisplay();
    oled.setFont(u8g2_font_unifont_t_chinese3);
    oled.setCursor(1, TITLE_LINE);
    oled.println(" [语音分类垃圾桶]");
    oled.drawHLine(0, TOP, 128);
    
    oled.sendBuffer();
}

