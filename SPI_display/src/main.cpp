#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HIGH 64
//软件 SPI 总线
//声明适用于使用软件 SPI 连接的 SSD1306 显示器（默认情况）：
#define OLED_MOSI 13
#define OLED_CLK 18
#define OLED_DC 2
#define OLED_CS 4
#define OLED_RESET 15

Adafruit_SSD1306 oled(SCREEN_WIDTH,SCREEN_HIGH,OLED_MOSI,OLED_CLK,OLED_DC,OLED_RESET,OLED_CS);
int i = 0;

void setup() {
  oled.begin();
  oled.clearDisplay();          //清除显示
  oled.drawFastHLine(5,5,123,SSD1306_WHITE); //显示水平线 参数：x 轴，y轴，长度，颜色
  oled.drawRect(5,7,27,27,SSD1306_WHITE); //绘制一个矩形 参数：x 轴，y 轴，长，宽，颜色
  oled.fillRect(5,35,27,27,SSD1306_WHITE);//绘制一个实心矩形 参数：同上
  oled.setCursor(33, 55);                 // 设置光标位置 参数：x轴，y轴
  oled.setTextColor(SSD1306_WHITE);
  oled.print("Hello,World!!");
  oled.drawFastVLine(3,5,57,SSD1306_WHITE); //显示垂直线 参数：x 轴，y轴，长度，颜色
  oled.drawFastVLine(1,5,57,SSD1306_WHITE); //显示垂直线 参数：x 轴，y轴，长度，颜色



  oled.display();               //显示内容

}

void loop() {


}
 

