#include <Arduino.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
#include "LiquidCrystal_I2C.h"
#include "net_time.h"
#include "now.h"
#include "esp_task_wdt.h"
#include "cloud.h"
#include <esp_now.h>

//初始化对应的四个溢出检测
#define IR1_pin 33
#define IR2_pin 32
#define IR3_pin 35
#define IR4_pin 34



esp_err_t yichu_send_result = false;





//多线程 溢出检测
void IR_yichu_task(void *pvParameters);

//多线程 着火检测
void ptc_zhaohuo_task(void *pvParameters);

//多线程 时间输出
void time_out_task(void *pvParameters);

//多线程 now 传输数据
void now_task(void *pvParameters);

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
//输出传感值到串口
void IR_puts();
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
  net_disconnect();

 
  //Serial.println("初始化MQTT");
  //MQTT_INIT();





  //将时间输出到串口上
  print_local_time_to_serial();
  //屏幕时间
  lcd1602_time(lcd1602,timeinfo);
  





  //初始化溢出传感器并且串口输出溢出传感器数字值
  IRinit();

//初始化并测试 now 
  Serial.println("开始初始化 NOW !!");
  now_init();


  //创建线程 溢出检测
  //xTaskCreate(IR_yichu_task,"yichujiance_task",8888,NULL,1,NULL);

  //创建线程 时间输出
  //xTaskCreate(time_out_task,"time_out",2048,NULL,1,NULL);

  //创建线程 物联网云平台
  //xTaskCreate(MQTT_task,"MQTT",8888,NULL,1,NULL);

  //创建线程 热敏检测


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
  Serial.println("传感器的值：");
  Serial.println(digitalRead(IR1_pin));
  Serial.println(digitalRead(IR2_pin));  
  Serial.println(digitalRead(IR3_pin));
  Serial.println(digitalRead(IR4_pin));
}












//输出传感值到串口
void IR_puts(){
  delay(1000);
Serial.println("传感器的值：");
  Serial.println(digitalRead(IR1_pin));
  Serial.println(digitalRead(IR2_pin));  
  Serial.println(digitalRead(IR3_pin));
  Serial.println(digitalRead(IR4_pin));

}

//溢出检测函数
void IR_yichu_jiance(){




//未遮挡为 1  遮挡为 0
     

  if (!digitalRead(IR1_pin))
  {
    Serial.println("一号垃圾桶已满！！");
    bin_yichu_static.BIN1 = true;
  }
  else
  {
    bin_yichu_static.BIN1 = false;
  }

  if(!digitalRead(IR2_pin))
  {
    Serial.println("二号垃圾桶已满！！");
    bin_yichu_static.BIN2 = true;
  }
  else
  {
    bin_yichu_static.BIN2 = false;
  }

  if(!digitalRead(IR3_pin))
  {
    Serial.println("三号垃圾桶已满！！");
    bin_yichu_static. BIN3 = true;
  }
  else
  {
    bin_yichu_static.BIN3 = false;
  }

  if (!digitalRead(IR4_pin))
  {
    Serial.println("四号垃圾桶已满！！");
    bin_yichu_static.BIN4 = true;
  }
  else
  {
    bin_yichu_static.BIN4 = false;
  }

Serial.print(".");
  
  //判断四个传感器其中有一个为 0 时 进行无线传输  遮挡为 0  
  if (!bin_yichu_static.BIN1 || !bin_yichu_static.BIN2 || !bin_yichu_static.BIN3 || !bin_yichu_static.BIN4)
  {
    if (!yichu_send_result)
    {
      yichu_send_result = now_send_data(bin_yichu_static);
      yichu_send_result = true;
    }
    else
    {
      yichu_send_result = false;
    }
  }
  
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


bin_static MY_bin;


//
void now_task(void *pvParameters){

while (true)
{
  Serial.println("多线程 now 传输数据");

  Serial.println(digitalRead(IR1_pin));
  Serial.println(digitalRead(IR2_pin));
  Serial.println(digitalRead(IR3_pin));
  Serial.println(digitalRead(IR4_pin));



  //遮挡时为结束 0 
  if ( !digitalRead(IR1_pin) || !digitalRead(IR2_pin) || !digitalRead(IR3_pin) || !digitalRead(IR4_pin))
  {
      MY_bin.BIN1 = digitalRead(IR1_pin);             
      MY_bin.BIN2 = digitalRead(IR2_pin);        
      MY_bin.BIN3 = digitalRead(IR3_pin);        
      MY_bin.BIN4 = digitalRead(IR4_pin);   
      MY_bin.flag = yichu_flag;//溢满     
  }
  else if(false)
    //这里是温度检测
    Serial.println("这里是温度检测");
  else
  {
      MY_bin.BIN1 = true;             
      MY_bin.BIN2 = true;        
      MY_bin.BIN3 = true;        
      MY_bin.BIN4 = true;   
      MY_bin.flag = 99;//无用 
  }

  now_send_data(MY_bin);
  

  // 添加适当的延迟，以防止任务占用 CPU 过多
  vTaskDelay(pdMS_TO_TICKS(100)); // 例如，延迟 100 毫秒
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
