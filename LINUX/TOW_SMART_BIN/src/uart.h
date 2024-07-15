#include <HardwareSerial.h>

 struct MYUART {
   bool yichu_BIN1 = true;
  bool yichu_BIN2 = true;
  bool yichu_BIN3 = true;
  bool yichu_BIN4 = true;
  bool zhaohuo_BIN1 = true;
  bool zhaohuo_BIN2 = true;
  bool zhaohuo_BIN3 = true;
  bool zhaohuo_BIN4 = true;
} MyUartData;

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

#define beeper_pin 4



// uart 初始化
void uart_init();

//uart 循环
void uart_loop();

// 多线程 uart发送数据
void uart_send(void *pvParameters);

//更新结构题数据函数
void updata_uart();

//发送数据
void send_uartData(const MYUART& data);

// 显示数据
void uart_data_print();



void uart_init(){
    //初始化 serial2 
    Serial.println("初始化 serial2");
    Serial2.begin(115200,SERIAL_8N1,16,17);
    Serial.println("初始化 beeper");
    pinMode(beeper_pin,OUTPUT);
    digitalWrite(beeper_pin,HIGH);
}





// 多线程 uart发送数据
void uart_send(void *pvParameters){
    while (1)
    {
        Serial.println("多线程 uart 函数中");

        send_uartData(MyUartData);
        vTaskDelay(1000);
        if (digitalRead(IR1_pin) == HIGH && digitalRead(IR2_pin) == HIGH && digitalRead(IR3_pin) == HIGH && digitalRead(IR4_pin) == HIGH &&
        digitalRead(ptc1_pin) == HIGH && digitalRead(ptc2_pin) == HIGH && digitalRead(ptc3_pin) == HIGH && digitalRead(ptc4_pin) == HIGH) {
            // 正常状态，停止蜂鸣器报警
            digitalWrite(beeper_pin, HIGH);
        } else {
            // 触发异常，垃圾桶状态异常
            Serial.println("垃圾桶状态异常");

            // 触发蜂鸣器报警
            digitalWrite(beeper_pin, LOW);
            delay(1000); // 持续报警 1 秒
            digitalWrite(beeper_pin, HIGH);
            delay(1000); // 停止报警 1 秒
        }
    }  
}

//更新结构题数据函数
void updata_uart(){
    MyUartData.yichu_BIN1 = digitalRead(IR1_pin);    
    MyUartData.yichu_BIN2 = digitalRead(IR2_pin);    
    MyUartData.yichu_BIN3 = digitalRead(IR3_pin);
    MyUartData.yichu_BIN4 = digitalRead(IR4_pin);
    MyUartData.zhaohuo_BIN1 = digitalRead(ptc1_pin);
    MyUartData.zhaohuo_BIN2 = digitalRead(ptc2_pin);
    MyUartData.zhaohuo_BIN3 = digitalRead(ptc3_pin);
    MyUartData.zhaohuo_BIN4 = digitalRead(ptc4_pin);

    Serial.println("更新后的数据");
    uart_data_print();
    Serial.println(" ");
    
}

void send_uartData(const MYUART& data) {
    
    //更新数据
    updata_uart();

    Serial2.write(reinterpret_cast<const uint8_t*>(&data), sizeof(data));
}





//输出溢满传感器值到串口监视器
void IR_puts(){
  delay(1000);
  Serial.println("溢出传感器的值(被遮挡为 0):");
  Serial.println(digitalRead(IR1_pin));
  Serial.println(digitalRead(IR2_pin));  
  Serial.println(digitalRead(IR3_pin));
  Serial.println(digitalRead(IR4_pin));
}



//输出温度传感器值到串口监视器
void PTC_puts(){
  Serial.println("温度传感器的值(达到预设值为 0)");
  Serial.println(digitalRead(ptc1_pin));
  Serial.println(digitalRead(ptc2_pin));
  Serial.println(digitalRead(ptc3_pin));
  Serial.println(digitalRead(ptc4_pin));
}

// 显示数据
void uart_data_print(){
    IR_puts();
    PTC_puts();
}