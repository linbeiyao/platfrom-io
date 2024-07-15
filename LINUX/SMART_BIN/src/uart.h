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


// uart 初始化
void uart_init();

//uart 循环
void uart_loop();

// 多线程 uart接受数据
void uart_recv();

// uart 接受数据的回调函数
void uart_recv_recall();



// uart 初始化
void uart_init(){

    Serial.println("  初始化串口");
    Serial2.begin(115200,SERIAL_8N1,16,17);

    // 设置 uart 回调函数
    Serial.println("  设置接受数据后的回调函数");
    Serial2.onReceive(uart_recv_recall);
}


//uart 循环
void uart_loop(){};



// uart 接受数据的回调函数
void uart_recv_recall(){

  Serial.println("uart 接受数据的回调函数中。。。");
    static uint8_t uart_buf[128];

    int count = Serial2.read(uart_buf,128);

    Serial.print("Recv:");
    Serial.println(count);

    if (count > 0)
    {
        memcpy(&MyUartData,uart_buf,sizeof(MyUartData));
    }
    else if(count == 0){
      Serial.println("空数据");
    }
    
        Serial.print("Bytes received:");
        Serial.println(sizeof(MyUartData));
        Serial.println(MyUartData.yichu_BIN1);
        Serial.println(MyUartData.yichu_BIN2);
        Serial.println(MyUartData.yichu_BIN3);
        Serial.println(MyUartData.yichu_BIN4);
        Serial.println(MyUartData.zhaohuo_BIN1);
        Serial.println(MyUartData.zhaohuo_BIN2);
        Serial.println(MyUartData.zhaohuo_BIN3);
        Serial.println(MyUartData.zhaohuo_BIN4);
        static int i = 0;

        Serial.print("这是第 ");
        Serial.print(i++);
        Serial.println(" 次接受 UART 数据！！");

}