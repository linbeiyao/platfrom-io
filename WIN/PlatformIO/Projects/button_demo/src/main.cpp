
#include <TFT_eSPI.h>
#include <Arduino.h>
#include "OneButton.h"


#define upButton_pin 27
#define downButton_pin 26
#define leftButton_pin 25
#define rightButton_pin 33
#define PIN_LED 2
 
OneButton upButton(upButton_pin, true);
OneButton downButton(downButton_pin,true);
OneButton leftButton(leftButton_pin,true);
OneButton rightButton(rightButton_pin,true);




//TFT
TFT_eSPI tft = TFT_eSPI(); 



//全局变量
int sWidth  = TFT_WIDTH;   //屏幕宽度
int sHeight = TFT_HEIGHT;  //屏幕高度
int bgColor = TFT_BLACK;   //背景颜色
int fgColor = TFT_BLUE;  //标题颜色
// 当前未显示最新的菜单 标志位 用于停止一直刷新 TFT
bool not_display_new_menu = false;


unsigned char keyValue = 0;
 
//定义按键结构体
typedef struct
{
  unsigned char index;
  unsigned char up;
  unsigned char down;
  unsigned char left;
  unsigned char right;
  void (*operation)(void);
} KEY_TABLE;
 
//定义日期时间结构体变量

 
unsigned char funIndex = 0;
void (*current)(void);
void menu11(void);
void menu12(void);
void menu21(void);
void menu22(void);
void menu23(void);
void menu31(void);
void menu32(void);
void menu33(void);
void menu34(void);
 

void showNum(String text, int16_t x, int16_t y, uint16_t color) {
  tft.setTextSize(1);
  tft.setTextColor(color);
  tft.drawString(text, x, y, 6);
  //tft.setFreeFont(&Orbitron_Light_32);
  //tft.drawString(text, x, y);
}






//定义按键操作数据
KEY_TABLE table[9] =
{
  {0, 0, 1, 0, 2, (*menu11)},     // 0      1-1
  {1, 0, 1, 1, 2, (*menu12)},     // 1      1-2

  {2, 2, 3, 0, 5, (*menu21)},     // 2      2-1
  {3, 2, 4, 0, 5, (*menu22)},     // 3      2-2
  {4, 3, 4, 0, 5, (*menu23)},     // 4      2-3

  {5, 5, 6, 2, 5, (*menu31)},     // 5      3-1
  {6, 5, 7, 2, 6, (*menu32)},     // 6      3-2
  {7, 6, 8, 2, 7, (*menu33)},     // 7      3-3
  {8, 7, 8, 2, 8, (*menu34)},     // 8      3-4
};


 
//一级菜单1
void menu11(void)
{
  tft.fillScreen(fgColor);
  showNum("11", 5, 100,bgColor );
  
  tft.setCursor(1,1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.println("k1---up");
  tft.println("k2---down");
  tft.println("k3---left");
  tft.println("k4---right");
  // tft.println("invertDisplay(1) * 2");
    // 颜色反转
  tft.invertDisplay(true);

  

  
}
 
//一级菜单2
void menu12(void)
{
 tft.fillScreen(fgColor); 
 showNum("12", 5, 100,bgColor );

  tft.setCursor(1,1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.println("k1---up");
  tft.println("k2---down");
  tft.println("k3---left");
  tft.println("k4---right");
  tft.invertDisplay(false);

}
 
//二级菜单1
void menu21(void)
{
  tft.fillScreen(fgColor); 
  showNum("21", 5, 100,bgColor );

  tft.setCursor(1,1);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);
  tft.println("k1---up");
  tft.println("k2---down");
  tft.println("k3---left");
  tft.println("k4---right");

  tft.println("tft.pushBlock(TFT_RED,50)");
  tft.pushBlock(TFT_RED,8100);
  sleep(1);
  tft.println("tft.pushBlock(TFT_YELLOW,60)");
  tft.pushBlock(TFT_YELLOW,8100);



}
 
//二级菜单2
void menu22(void){

tft.fillScreen(fgColor);
showNum("22", 5, 100,bgColor );

tft.setCursor(1,1);
tft.setTextSize(1);
tft.setTextColor(TFT_WHITE,TFT_BLACK);
tft.println("k1---up");
tft.println("k2---down");
tft.println("k3---left");
tft.println("k4---right");

}
 
//二级菜单3
void menu23(void)
{
tft.fillScreen(fgColor);
showNum("23", 5, 100,bgColor );

tft.setCursor(1,1);
tft.setTextSize(1);
tft.setTextColor(TFT_WHITE,TFT_BLACK);
tft.println("k1---up");
tft.println("k2---down");
tft.println("k3---left");
tft.println("k4---right");


 
}
 
//三级菜单1
void menu31(void)
{
tft.fillScreen(fgColor);
showNum("31", 5, 100,bgColor );

tft.setCursor(1,1);
tft.setTextSize(1);
tft.setTextColor(TFT_WHITE,TFT_BLACK);
tft.println("k1---up");
tft.println("k2---down");
tft.println("k3---left");
tft.println("k4---right");


}
 
//三级菜单2
void menu32(void)
{
tft.fillScreen(fgColor);
showNum("32", 5, 100,bgColor );

tft.setCursor(1,1);
tft.setTextSize(1);
tft.setTextColor(TFT_WHITE,TFT_BLACK);
tft.println("k1---up");
tft.println("k2---down");
tft.println("k3---left");
tft.println("k4---right");


}
 
//三级菜单3
void menu33(void)
{
tft.fillScreen(fgColor);
showNum("33", 5, 100,bgColor );

tft.setCursor(1,1);
tft.setTextSize(1);
tft.setTextColor(TFT_WHITE,TFT_BLACK);
tft.println("k1---up");
tft.println("k2---down");
tft.println("k3---left");
tft.println("k4---right");

}
 
//三级菜单4
void menu34(void)
{
tft.fillScreen(fgColor);
showNum("34", 5, 100,bgColor );

tft.setCursor(1,1);
tft.setTextSize(1);
tft.setTextColor(TFT_WHITE,TFT_BLACK);
tft.println("k1---up");
tft.println("k2---down");
tft.println("k3---left");
tft.println("k4---right");

 
}
 
//按键扫描函数

//*******************按键控制*************************
void doubleclick()
{
  tft.fillScreen(TFT_ORANGE); // 清屏
  tft.print("k1:");tft.println(upButton.getNumberClicks());
  tft.print("k2:");tft.println(downButton.getNumberClicks());
  tft.print("k3:");tft.println(leftButton.getNumberClicks());
  tft.print("k4:");tft.println(rightButton.getNumberClicks());
}
void click()
{
 funIndex = table[funIndex].right;
}
void longclick()
{
funIndex = table[funIndex].left;
}


void up_click(){
  funIndex = table[funIndex].up;
  Serial.println("up_click");
  not_display_new_menu = true;
}
void down_click(){
  funIndex = table[funIndex].down;
  Serial.println("down_click");
  not_display_new_menu = true;
}
void right_click(){
  funIndex = table[funIndex].right;
  Serial.println("right_click");
  not_display_new_menu = true;
}
void left_click(){
  funIndex = table[funIndex].left;
  Serial.println("left_click");
  not_display_new_menu = true;
}


//*********************END***************************



void setup()
{
    //初始化TFT
  tft.init();
  tft.setRotation(2);
  //缺少这个位图颜色会反转
  tft.setSwapBytes(true);
  tft.setTextSize(1);
  tft.setTextColor(bgColor);
  tft.fillScreen(fgColor);

  pinMode(upButton_pin,INPUT);
  pinMode(downButton_pin,INPUT);
  pinMode(leftButton_pin,INPUT);
  pinMode(rightButton_pin,INPUT);

  upButton.attachClick(up_click);
  upButton.attachDoubleClick(doubleclick);
  // upButton.attachLongPressStart(longclick);
  downButton.attachClick(down_click);
  leftButton.attachClick(left_click);
  rightButton.attachClick(right_click);

  // tft.pushColor(TFT_BLACK);
  // sleep(500);
  // tft.pushColor(TFT_YELLOW);
  // sleep(500);
  // tft.pushColor(TFT_RED);
  // sleep(500);
  // tft.pushColor(TFT_BLACK);


  
}
 


void loop()
{
  
 
  // 按键检测
  upButton.tick();
  downButton.tick();
  leftButton.tick();
  rightButton.tick();

  current = table[funIndex].operation;//根据需要获取对应需要执行的函数

  if (not_display_new_menu)
  {
   (*current)();//执行获取到的函数
   not_display_new_menu = false;
  }
  


}
