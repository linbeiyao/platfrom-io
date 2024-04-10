#include <esp_now.h>
#include <WiFi.h>

typedef struct struct_message
{
    char a[32];
    int b;
    float c;
    bool d;
} struct_message;

//接受信息结构体的标志
#define yichu_flag 0
#define zhaohuo_flag 1

//垃圾桶传感器状态
struct bin_static 
{
  int flag;
  bool BIN1;
  bool BIN2;
  bool BIN3;
  bool BIN4;
}MY_bin;


const char *ssid = "123456";//wifi姓名
const char *passwd = "88888888";










//now 协议初始化函数
void now_init();
//wifi的初始化
void wifi_init();
//接收到数据的回调函数
void OnDataRecv(const uint8_t * mac,const uint8_t *incomingData,int len);




bin_static myData;

void now_init(){
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK )
    {
        Serial.println("初始化 ESP-NOW 时出错");
    }

    // 一旦ESPNow成功初始化，我们将注册recv CB
    // 获取recv打包器信息
    esp_now_register_recv_cb(OnDataRecv);
}

void wifi_init(){

    Serial.println("WiFi 开始初始化！！");
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高反应速度
    WiFi.begin(ssid,passwd);//连接wifi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!!");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}



//接收到数据的回调函数
void OnDataRecv(const uint8_t * mac,const uint8_t *incomingData,int len){
    memcpy(&MY_bin,incomingData,sizeof(MY_bin));
    Serial.println(sizeof(MY_bin));
    Serial.println(len);

    Serial.println("已收到数据.....");
    delay(1000);
    


}



