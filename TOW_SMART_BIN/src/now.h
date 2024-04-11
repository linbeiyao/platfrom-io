#include <esp_now.h>
#include <WiFi.h>

//传送信息结构体的标志
#define yichu_flag 0
#define zhaohuo_flag 1

//初始化对应的四个溢出检测
#define IR1_pin 33
#define IR2_pin 32
#define IR3_pin 35
#define IR4_pin 34

//预定义对应的四个温度检测
#define ptc1_pin  13
#define ptc2_pin  14
#define ptc3_pin  27
#define ptc4_pin  23

//垃圾桶传感器状态
struct bin_static 
{
  int flag;
  bool yichu_BIN1;
  bool yichu_BIN2;
  bool yichu_BIN3;
  bool yichu_BIN4;
  bool zhaohuo_BIN1;
  bool zhaohuo_BIN2;
  bool zhaohuo_BIN3;
  bool zhaohuo_BIN4;
} ;

bin_static MY_bin;


// 初始化函数
void now_init();

void OnDataSent(const uint8_t *mac_addr,esp_now_send_status_t status);

//多线程 now 传输数据
void now_task(void *pvParameters);


//接受数据的单片机的 MAC 地址  64:B7:08:61:C7:B4
uint8_t broadcastAddress[] = {0x64,0xB7,0x08,0x61,0xC7,0xB4};

esp_now_peer_info peerInfo;

//发送数据时的回调函数
void OnDataSent(const uint8_t *mac_addr,esp_now_send_status_t status){
    Serial.print("\r\n最后一个数据包发送状态:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "发送成功" : "发送失败");
    
}

// 初始化函数
void now_init(){
    //设置设备的模式
    WiFi.mode(WIFI_STA);
    
    //初始化 ESP-NOW
    if (esp_now_init() !=ESP_OK)
    {
        Serial.println("初始化 esp_now 时出错！");
        return;
    }
    else{
        Serial.println("初始化 ESP-NOW 成功！！");
    }

    
    // 一旦 ESPNow 成功初始化，我们将注册 Send CB to
    // 获取已发送数据包的状态
     esp_now_register_send_cb(OnDataSent);

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}




//发送数据   
//检测所有针脚的状态 如果有一个针脚为异常也就是为 0 时 发送数据
void now_send_data(bin_static &bin_static){
    //更新 MY_bin 的状态数据



    //发送数据并且获得 是否发送成功的结果
    esp_err_t result = esp_now_send(broadcastAddress,(uint8_t*) &bin_static,sizeof(bin_static));

    if (result == ESP_OK) {
        Serial.println("发送成功！！");
    }
    else {
        Serial.println("发送数据过程中出错！！");
    }

    delay(2000);

}



