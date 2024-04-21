#include <ThingsCloudWiFiManager.h>
#include <ThingsCloudMQTT.h>

//======================================================
// 设置 ssid / password，连接到你的 WiFi AP
const char *MQTT_ssid = "123456";
const char *MQTT_password = "88888888";
// 在 ThingsCloud 控制台的设备详情页中，复制以下设备连接信息
// https://console.thingscloud.xyz
#define THINGSCLOUD_MQTT_HOST "sh-1-mqtt.iot-api.com"
#define THINGSCLOUD_DEVICE_ACCESS_TOKEN "5l346910dz2pvdlr"
#define THINGSCLOUD_PROJECT_KEY "zQ8kxpV7zH"
//======================================================



ThingsCloudMQTT MQTT_client(
    THINGSCLOUD_MQTT_HOST,
    THINGSCLOUD_DEVICE_ACCESS_TOKEN,
    THINGSCLOUD_PROJECT_KEY
);
//存储上报数据的json对象
DynamicJsonDocument obj(512);



// 上报数据的间隔时间计时器
unsigned long timer1 = millis();
// 设置定时上报数据的时间间隔，单位是 ms。免费版项目请务必大于30秒，否则设备可能会被限连。
const int report_interval = 35000;



void MQTT_INIT(){

    //允许 SDK 的日志输出
    MQTT_client.enableDebuggingMessages();

    //连接 WIFI AP
    MQTT_client.setWifiCredentials(MQTT_ssid,MQTT_password);
}

//读取并发布传感器数据到 ThingsCloud
void pubSensors()
{

    Serial.println("向物联网云平台发送数据");
    //四个热敏电阻传感器的数据值
    bool ptc1 = myData.zhaohuo_BIN1,ptc2 = myData.zhaohuo_BIN2,ptc3 = myData.zhaohuo_BIN3,ptc4 = myData.zhaohuo_BIN4;
    bool yichu_bin1 = myData.yichu_BIN1, yichu_bin2 = myData.yichu_BIN2, yichu_bin3 = myData.yichu_BIN3 , yichu_bin4 = myData.yichu_BIN4;
    
    obj["yichu_bin1"] = yichu_bin1;
    obj["yichu_bin2"] = yichu_bin2;
    obj["yichu_bin3"] = yichu_bin3;
    obj["yichu_bin4"] = yichu_bin4;
    obj["ptc1"] = ptc1;
    obj["ptc2"] = ptc2;    
    obj["ptc3"] = ptc3;
    obj["ptc4"] = ptc4;
    char attributes[512];

    

    Serial.println("垃圾桶状态异常");
        
        

    

    serializeJson(obj, attributes);
    // 调用属性上报方法
    MQTT_client.reportAttributes(attributes);
}


// 必须实现这个回调函数，当 MQTT 连接成功后执行该函数。
void onMQTTConnect(){
    //延时5s上报首次传感器数据
    Serial.println("MQTT 成功连接!!");
    MQTT_client.executeDelayed(1000 * 5,[](){
        pubSensors();
    });

}


void MQTT_loop(){

    Serial.println("MQTT.loop");
    MQTT_client.loop();

  // 按间隔时间上报传感器数据
  
  if (millis() - timer1 > report_interval)
  {
    Serial.println("按间隔时间上报传感器数据");
    
    timer1 = millis();
    pubSensors();
    }
    


    pubSensors();


}
