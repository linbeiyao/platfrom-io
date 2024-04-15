#include <esp_now.h>
#include <WiFi.h>




// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
   bool yichu_BIN1 = true;
  bool yichu_BIN2 = true;
  bool yichu_BIN3 = true;
  bool yichu_BIN4 = true;
  bool zhaohuo_BIN1 = true;
  bool zhaohuo_BIN2 = true;
  bool zhaohuo_BIN3 = true;
  bool zhaohuo_BIN4 = true;
} struct_message;

// Create a struct_message called myData
struct_message myData;
struct_message lastData; // 上一次发送的传感器状态

const char *ssid = "123456";//wifi姓名
const char *passwd = "88888888";




//now 协议初始化函数
void now_init();
//wifi的初始化
//void wifi_init();
//接收到数据的回调函数
void OnDataRecv(const uint8_t * mac,const uint8_t *incomingData,int len);

//溢满异常
void yiman_warn();
//温度异常
void wendu_warn();





void now_init(){
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK )
    {
        Serial.println("初始化 ESP-NOW 时出错");
    }
    else{
        Serial.println("初始化 ESP-NOW 成功！！");
    }

    // 一旦ESPNow成功初始化，我们将注册recv CB
    // 获取recv打包器信息
    esp_now_register_recv_cb(OnDataRecv);
}

void wifi_init(){

    Serial.println("WiFi 开始初始化！！");
    WiFi.setSleep(false); //关闭STA模式下wifi休眠，提高反应速度
    //WiFi.begin(ssid,passwd);//连接wifi
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connected!!");
    Serial.print("IP Address:");
    Serial.println(WiFi.localIP());
}


// callback function that will be executed when data is received
// 回调函数这里只负责更新数据 ， 异常检测由 main.cpp 文件中的多线程任务来完成
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  Serial.print("Bytes received:");
  Serial.println(len);
  Serial.println(myData.yichu_BIN1);
  Serial.println(myData.yichu_BIN2);
  Serial.println(myData.yichu_BIN3);
  Serial.println(myData.yichu_BIN4);
  Serial.println(myData.zhaohuo_BIN1);
  Serial.println(myData.zhaohuo_BIN2);
  Serial.println(myData.zhaohuo_BIN3);
  Serial.println(myData.zhaohuo_BIN4);
  static int i = 0;

  Serial.print("这是第 ");
  Serial.print(i++);
  Serial.println(" 次接受 now 数据！！");
}

