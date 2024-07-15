#include <WiFi.h>
// #include "LiquidCrystal_I2C.h"

//wifi账号和密码
const char *ssid = "123456";
const char *passwd = "88888888";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 8 * 3600;
const int daylightOffset_sec = 0;


//网络初始化
void net_init();

//断开网络连接
void net_disconnect();

//获取网络时间
void get_net_time();

//把时间输出到串口
void print_local_time_to_serial();

//把时间输出到1602
struct tm print_local_time_to_lcd1602();










void net_init(){
    WiFi.begin(ssid,passwd);
    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFi connected!");
    Serial.print("WIFI 地址：");
    Serial.println(WiFi.localIP());    
}

void get_net_time(){
    configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);
    print_local_time_to_serial();         //把时间输出到串口
}

void print_local_time_to_serial(){
    struct tm timeinfo;

    if(!getLocalTime(&timeinfo)){

        Serial.println("时间输出错误");
        net_init();
        get_net_time();
        net_disconnect();
        return ;
    }
    Serial.println(&timeinfo,"%F %T %A");
} 




struct tm  print_local_time_to_lcd1602(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        net_init();
        get_net_time();
        net_disconnect();
    }
    return timeinfo;

}

void net_disconnect(){
    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WiFi disconnected!");
}

