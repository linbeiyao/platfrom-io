#include <ThingsCloudWiFiManager.h>
#include <ThingsCloudMQTT.h>


// ===========================================
// WIFI 的账号和密码
const char *MQTT_ssid = "123456";
const char *MQTT_passwd = "88888888";
// 设备连接信息
#define THINGSCLOUD_MQTT_HOST "sh-1-mqtt.iot-api.com" 
#define THINGSCLOUD_DEVICE_ACCESS_TOKEN "ez3glrjwkpom3k6l"
#define THINGSCLOUD_PROJECT_KEY "hHw1jrGc9v"

// All_led_num
int all[] = {2,15,4,16,17,5,18,19,21,0};

// ===========================================


ThingsCloudMQTT MQTT_client(
    THINGSCLOUD_MQTT_HOST,
    THINGSCLOUD_DEVICE_ACCESS_TOKEN,
    THINGSCLOUD_PROJECT_KEY
);
//存储上报数据的json对象
DynamicJsonDocument obj(512);
// 上报数据的间隔时间计时器
unsigned long timer1 = millis();
// 设置定时上报数据的时间间隔，单位是 ms。免费版项目请务必大于35秒，否则设备可能会被限连。
const int report_interval = 35000;
// 定义函数 用来检测楼层所有灯的状态，全亮为 ture
boolean floor_static();



void MQTT_INIT(){

    //允许 SDK 的日志输出
    MQTT_client.enableDebuggingMessages();

    //连接 WIFI AP
    MQTT_client.setWifiCredentials(MQTT_ssid,MQTT_passwd);
}



//读取并发布传感器数据到 ThingsCloud
void pubSensors()
{

    obj["led1"] = (boolean)digitalRead(all[0]);    
    obj["led2"] = (boolean)digitalRead(all[1]);
    obj["led3"] = (boolean)digitalRead(all[2]);
    obj["led4"] = (boolean)digitalRead(all[3]);
    obj["led5"] = (boolean)digitalRead(all[4]);
    obj["led6"] = (boolean)digitalRead(all[5]);
    obj["led7"] = (boolean)digitalRead(all[6]);
    obj["led8"] = (boolean)digitalRead(all[7]);    
    obj["led9"] = (boolean)digitalRead(all[8]);   
    obj["floor1_led"] = floor_static();
    obj["floor2_led"] = floor_static();
    obj["floor3_led"] = floor_static();
    char attributes[512];  

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
    
    // pubSensors();
}



