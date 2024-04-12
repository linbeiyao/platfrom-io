
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
   bool yichu_BIN1;
  bool yichu_BIN2;
  bool yichu_BIN3;
  bool yichu_BIN4;
  bool zhaohuo_BIN1;
  bool zhaohuo_BIN2;
  bool zhaohuo_BIN3;
  bool zhaohuo_BIN4;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
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
  Serial.println(i++);


}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

}