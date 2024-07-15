#include <Arduino.h>
#include "LiquidCrystal_I2C.h"

//设置 LCD1602 的地址、列数、行数
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
lcd.init();

//打印内容
lcd.home();
lcd.blink();
lcd.backlight();
lcd.print("Hello,world!");
lcd.display();
lcd.cursor();
}

void loop() {

}

