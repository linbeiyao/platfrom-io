#include <Arduino.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include "LiquidCrystal_I2C.h"
#include "net_time.h"
#include "now.h"
#include "esp_task_wdt.h"
#include "cloud.h"
#include <esp_now.h>



esp_err_t yichu_send_result = false;





//多线程 溢出检测
void IR_yichu_task(void *pvParameters);

//多线程 着火检测
void ptc_zhaohuo_task(void *pvParameters);

//多线程 时间输出
void time_out_task(void *pvParameters);


//多线程 物联网云平台
void MQTT_task(void *pvParameters);


typedef LiquidCrystal_I2C LCD;

//实例化 1602 屏幕
 LCD lcd1602(0x27,16,2);

//屏幕初始化
void LCD_INIT(LCD &lcd);
//屏幕时间
void lcd1602_time(LCD &lcd,tm &timeinfo);
//溢出传感器初始化
void IRinit();
//温度传感器初始化
void PTCinit();
//输出传感值到串口
void IR_puts();
//输出温度传感器值到串口监视器
void PTC_puts();
//溢出检测函数
void IR_yichu_jiance();

//时间结构
struct tm timeinfo;


void setup() {
  //1602初始化
  LCD_INIT(lcd1602);
  Serial.begin(115200);

  //初始化网络
  net_init();
  //获取网络时间
  get_net_time();
  //断开网络
  //net_disconnect();

  Serial.println("开始初始化 NOW !!");
  now_init();
  Serial.println("初始化MQTT");
  MQTT_INIT();


  //将时间输出到串口上
  print_local_time_to_serial();
  //屏幕时间
  lcd1602_time(lcd1602,timeinfo);
  

  //初始化溢出传感器并且串口输出溢出传感器数字值
  IRinit();
  //初始化温度传感器并且串口输出温度传感器的数值
  PTCinit();

//初始化并测试 now 
  


  //创建线程 时间输出
  xTaskCreate(time_out_task,"time_out",2048,NULL,1,NULL);

  //创建线程 物联网云平台
  xTaskCreate(MQTT_task,"MQTT",8888,NULL,1,NULL);

  


  //创建线程 now 传输数据
  xTaskCreate(now_task,"now",8888,NULL,1,NULL);
}

void loop() {
}

//屏幕初始化 
void LCD_INIT(LCD &lcd)
{
  //初始化
  lcd.init();

  //显示内容//
  lcd.home();     //恢复光标位置
  // lcd.blink();    
  lcd.backlight();
  lcd.print(" Hello,World!!!");
  lcd.setCursor(0,1);
  lcd.print("  [SMART_BIN]");
  lcd.display();
  




}

//屏幕时间
void lcd1602_time(LCD &lcd,tm &timeinfo){

  timeinfo = print_local_time_to_lcd1602();

  //清空屏幕显示时间
  lcd.clear();
  lcd.home();
  lcd.print("Date:");
  lcd.print(&timeinfo,"%F");
  lcd.setCursor(0,1);
  lcd.print("Time:");
  lcd.print(&timeinfo,"%T");
}

//初始化溢出传感器并且串口输出溢出传感器数字值
void IRinit(){
  pinMode(IR1_pin,INPUT);
  pinMode(IR2_pin,INPUT);
  pinMode(IR3_pin,INPUT);
  pinMode(IR4_pin,INPUT);
  Serial.println("溢出传感器的值(被遮挡为 0):");
  Serial.println(digitalRead(IR1_pin));
  Serial.println(digitalRead(IR2_pin));  
  Serial.println(digitalRead(IR3_pin));
  Serial.println(digitalRead(IR4_pin));
}

void PTCinit(){
  pinMode(ptc1_pin, INPUT);  
  pinMode(ptc2_pin, INPUT);
  pinMode(ptc3_pin, INPUT);
  pinMode(ptc4_pin, INPUT);  
  Serial.println("温度传感器的值(达到预设值为 0)");
  Serial.println(digitalRead(ptc1_pin));
  Serial.println(digitalRead(ptc2_pin));
  Serial.println(digitalRead(ptc3_pin));
  Serial.println(digitalRead(ptc4_pin));
}












//输出溢满传感器值到串口监视器
void IR_puts(){
  delay(1000);
  Serial.println("溢出传感器的值(被遮挡为 0):");
  Serial.println(digitalRead(IR1_pin));
  Serial.println(digitalRead(IR2_pin));  
  Serial.println(digitalRead(IR3_pin));
  Serial.println(digitalRead(IR4_pin));
}



//输出温度传感器值到串口监视器
void PTC_puts(){
  Serial.println("温度传感器的值(达到预设值为 0)");
  Serial.println(digitalRead(ptc1_pin));
  Serial.println(digitalRead(ptc2_pin));
  Serial.println(digitalRead(ptc3_pin));
  Serial.println(digitalRead(ptc4_pin));
}

//多线程任务 溢出检测
void IR_yichu_task(void *pvParameters){


  while (true)
  {
    
    IR_puts();
    IR_yichu_jiance();
    
    // 添加适当的延迟，以防止任务占用 CPU 过多
    vTaskDelay(pdMS_TO_TICKS(100)); // 例如，延迟 100 毫秒
  }
}



//多线程任务 时间输出
void time_out_task(void *pvParameters){


  while (true)
  {
    
    Serial.println("屏幕输出时间");
     lcd1602_time(lcd1602,timeinfo);


   // 添加适当的延迟，以防止任务占用 CPU 过多
  vTaskDelay(pdMS_TO_TICKS(1000)); // 例如，延迟 100 毫秒
  }
  
  
}


//多线程 物联网云平台
void MQTT_task(void *pvParameters){
  //必须有一个死循环
    while (true)
  {
    Serial.println("多线程 物联网云平台");
    Serial.println("");
    //检测时间间隔（ 35秒 ） 提交数据
    MQTT_loop();

    // 添加适当的延迟，以防止任务占用 CPU 过多
    vTaskDelay(pdMS_TO_TICKS(1000 * 10)); // 例如，延迟 100 毫秒
  }
}












