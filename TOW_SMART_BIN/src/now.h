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
  bool yichu_BIN1;
  bool yichu_BIN2;
  bool yichu_BIN3;
  bool yichu_BIN4;
  bool zhaohuo_BIN1;
  bool zhaohuo_BIN2;
  bool zhaohuo_BIN3;
  bool zhaohuo_BIN4;
};

bin_static MY_bin;
bin_static last_bin; // 上一次发送的传感器状态

// 初始化函数
void now_init();
// 二次初始化函数
esp_err_t now_tow_init();

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

//更新 NOW 数据
void updata_now();

//多线程 now 传输数据
void now_task(void *pvParameters);

// WIFI 信道扫描
// 功能：扫描指定WIFI目前信道
int32_t getWiFiChannel(const char *ssid);

//接受数据的单片机的 MAC 地址  64:B7:08:61:C7:B4
uint8_t broadcastAddress[] = {0x64,0xB7,0x08,0x61,0xC7,0xB4};

esp_now_peer_info peerInfo;

//发送数据时的回调函数
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status){
    Serial.print("\r\n最后一个数据包发送状态:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "发送成功" : "发送失败");
}


// 初始化函数
void now_init(){
    //设置设备的模式
    WiFi.mode(WIFI_AP_STA);
    

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
    //peerInfo.channel =  0;  
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return ; 
    }
}

//发送数据   
//检测所有针脚的状态 如果有一个针脚为异常也就是为 0 时 发送数据
void now_send_data(){
    // 更新 MY_bin 的状态数据
    updata_now();

    // 只有当传感器状态发生变化时才发送数据
    if (memcmp(&MY_bin, &last_bin, sizeof(bin_static)) != 0) {
        // 发送数据并且获得 是否发送成功的结果
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t*) &MY_bin, sizeof(MY_bin));
        if (result == ESP_OK) {
            Serial.println("发送成功！！");
            // 更新上一次发送的传感器状态
            memcpy(&last_bin, &MY_bin, sizeof(bin_static));
        }
        else {
            Serial.println("发送数据过程中出错！！");
        }
    }
}

//更新结构题数据函数
void updata_now(){
    MY_bin.yichu_BIN1 = digitalRead(IR1_pin);    
    MY_bin.yichu_BIN2 = digitalRead(IR2_pin);    
    MY_bin.yichu_BIN3 = digitalRead(IR3_pin);
    MY_bin.yichu_BIN4 = digitalRead(IR4_pin);
    MY_bin.zhaohuo_BIN1 = digitalRead(ptc1_pin);
    MY_bin.zhaohuo_BIN2 = digitalRead(ptc2_pin);
    MY_bin.zhaohuo_BIN3 = digitalRead(ptc3_pin);
    MY_bin.zhaohuo_BIN4 = digitalRead(ptc4_pin);
}

//多线程 now 传输数据
void now_task(void *pvParameters){
   while (true)
   {
    
    // if (now_tow_init() != ESP_OK)
    // {
    //     Serial.println("初始化 esp_now 时出错！");
    //     Serial.println("重新初始化！！");
    //     break;
    // }


    //now_init();

    now_send_data();
    vTaskDelay(1000);
   }
}








// WIFI 信道扫描
// 功能：扫描指定WIFI目前信道
int32_t getWiFiChannel(const char *ssid){
    if(int32_t n = WiFi.scanNetworks())
    {
        for(uint8_t i = 0; i < n; i++){
            if(!strcmp(ssid,WiFi.SSID(i).c_str())){
                return WiFi.channel(i);
            }
        }
    }
}