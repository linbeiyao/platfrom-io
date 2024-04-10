#include <esp_now.h>
#include <WiFi.h>

//传送信息结构体的标志
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
} bin_zhaohuo_static = {   //着火状态的结构体
  zhaohuo_flag, 
  false, 
  false, 
  false, 
  false
} , bin_yichu_static = {   //溢出状态的结构体
  yichu_flag, 
  false, 
  false, 
  false, 
  false
};

// 初始化函数
void now_init();

void OnDataSent(const uint8_t *mac_addr,esp_now_send_status_t status);

//接受数据的单片机的 MAC 地址  64:B7:08:61:C7:B4
uint8_t broadcastAddress[] = {0x64,0xB7,0x08,0x61,0xC7,0xB4};

//发送数据的结构
typedef struct struct_message{

    char a[32];
    int b;
    float c;
    bool d;
}struct_message;

//创建一个存储数据的结构
struct_message myData;

//esp_now_peer_info 结构体的参数如下：
//对等的 MAC 地址
//用于加密数据的 ESPNOW 对等本地主密钥
//对等方用于发送/接收 ESPNOW 数据的 Wi-Fi 通道。 如果值为 0，使用电台或 softap 所在的当前频道。 否则，必须是设置为电台或 softap 所在的频道。
//对等方用于发送/接收 ESPNOW 数据的 Wi-Fi 接口
//该对等方发送/接收的 ESPNOW 数据是否加密
//ESPNOW 对等私有数据

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
    
    // 一旦 ESPNow 成功初始化，我们将注册 Send CB to
    // 获取已发送数据包的状态
     esp_now_register_send_cb(OnDataSent);


// /*
// *      typedef struct esp_now_peer_info {
// *       uint8_t peer_addr[ESP_NOW_ETH_ALEN];    /**< ESPNOW peer MAC address that is also the MAC address of station or softap */
//        uint8_t lmk[ESP_NOW_KEY_LEN];           /**< ESPNOW peer local master key that is used to encrypt data */
//         uint8_t channel;                        /**< Wi-Fi channel that peer uses to send/receive ESPNOW data. If the value is 0,
//                                                     use the current channel which station or softap is on. Otherwise, it must be
//                                                     set as the channel that station or softap is on. */
//         wifi_interface_t ifidx;                 /**< Wi-Fi interface that peer uses to send/receive ESPNOW data */
//         bool encrypt;                           /**< ESPNOW data that this peer sends/receives is encrypted or not */
//         void *priv;                             /**< ESPNOW peer private data */
//     } esp_now_peer_info_t;
// */



    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

}



//实例——发送数据
void send_data(){
    strcpy(myData.a, "THIS IS A CHAR");
    myData.b = random(1,20);
    myData.c = 1.2;
    myData.d = false;
    
    Serial.println("测试数据！！");

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
        Serial.println("发送成功！！");
    }
    else {
        Serial.println("发送数据过程中出错！！");
    }
    delay(2000);
}


//发送数据
esp_err_t now_send_data(bin_static &bin_static){
    esp_err_t result = esp_now_send(broadcastAddress,(uint8_t*) &bin_static,sizeof(bin_static));


    if (bin_static.flag == yichu_flag)
    {
        Serial.println("发送溢出传感器的参数");
    }
    else if (bin_static.flag == zhaohuo_flag)
    {
        Serial.println("发送着火传感器的参数");
    }
    
    

    if (result == ESP_OK) {
        Serial.println("发送成功！！");

    }
    else {
        Serial.println("发送数据过程中出错！！");
    }
    delay(2000);

    return result;
}



void now_init_tow(){

//设置设备的模式
    WiFi.mode(WIFI_STA);
    
    //初始化 ESP-NOW
    if (esp_now_init() !=ESP_OK)
    {
        Serial.println("初始化 esp_now 时出错！");
        return;
    }
    
    // 一旦 ESPNow 成功初始化，我们将注册 Send CB to
    // 获取已发送数据包的状态
     esp_now_register_send_cb(OnDataSent);


}

