#include <Arduino.h>
#include "lib.h"
#include "thingscloud.h" 


#define loudao_led_num 4

// All_led_num
// int all[] = {2,15,4,16,17,5,18,19,21,0};

// fool_led_num
int fool_led[][loudao_led_num] = {
                        {2,15,4,0},
                        {16,17,5,0},
                        {18,19,21,0},
                        {22,0}
                    };

// 人体传感器 
#define hbs_1 34
#define hbs_2 35
#define hbs_3 32




class floor_way
{
private:
    int led_arr[loudao_led_num];
    int led_pin1,led_pin2,led_pin3; 
    int hc_pin;
    int guangxian_pin;
    char ch;
public:
    // 构造函数 析构函数
    floor_way(int arr[loudao_led_num],int hc_pin,int guangxian_pin);
    ~floor_way();

    // 光线传感器 控制 led
    void hc_led(){

    if(!digitalRead(this->hc_pin)){ // 使用 this-> 来访问成员变量 hc_pin
        Serial.printf("检测传感器引脚为：%d 引脚状态为: %d \n",this->hc_pin,digitalRead(this->hc_pin));
  
        Serial.printf("被设置高电压的引脚为：%d %d %d\n",this->led_arr[0] ,this->led_arr[1] ,this->led_arr[2] );
        digitalWrite(this->led_arr[0], HIGH);
        digitalWrite(this->led_arr[1], HIGH);
        digitalWrite(this->led_arr[2], HIGH);

    }
    else{

        Serial.print("检测传感器引脚为：");
        Serial.println(this->hc_pin);
        Serial.printf("被设置第电压的引脚为：%d %d %d\n",this->led_arr[0] ,this->led_arr[1] ,this->led_arr[2] );

        digitalWrite(this->led_arr[0], LOW);
        digitalWrite(this->led_arr[1], LOW);
        digitalWrite(this->led_arr[2], LOW);

    }
}

    // 设置 LED 亮
    void set_led_hgih(){
        Serial.printf("设置 LED 亮 针脚为：%d %d %d \n",this->led_arr[0],this->led_arr[1],this->led_arr[2]);
        digitalWrite(this->led_arr[0],HIGH);        
        digitalWrite(this->led_arr[1],HIGH);        
        digitalWrite(this->led_arr[2],HIGH);            
    }

    // 设置 LED 灭
    void set_led_low(){
        Serial.printf("设置 LED 亮 针脚为：%d %d %d \n",this->led_arr[0],this->led_arr[1],this->led_arr[2]);
        digitalWrite(this->led_arr[0],LOW);        
        digitalWrite(this->led_arr[1],LOW);        
        digitalWrite(this->led_arr[2],LOW);            
    }

    // 打印类的数据
    void print_data(){
        Serial.print("该楼道灯的IO针脚为:");
        Serial.printf(" %d %d %d \n",this->led_arr[0],this->led_arr[1],this->led_arr[2]);
        Serial.printf("该楼道灯的状态为 :%d %d %d \n",digitalRead(led_arr[0]),digitalRead(led_arr[1]),digitalRead(led_arr[2]));
        Serial.printf("该楼道的人体传感器的io针脚为: %d ,状态为：%d\n",this->hc_pin,digitalRead(this->hc_pin));
    }
};

/// @brief 
/// @param arr 
/// @param hc_pin 
/// @param guangxian_pin 
// 构造函数
floor_way::floor_way(int arr[loudao_led_num],int hc_pin,int guangxian_pin)
{
     
    this->led_arr[0] = arr[0];
    this->led_arr[1] = arr[1];
    this->led_arr[2] = arr[2];
    this->hc_pin =  hc_pin;
    this->guangxian_pin = guangxian_pin; 

    led_pin1 = led_arr[0];
    led_pin2 = led_arr[1];
    led_pin3 = led_arr[2];

    Serial.printf("led_pin1 = %d\nled_pin2 = %d\nled_pin3 = %d",led_pin1,led_pin2,led_pin3);

    // led_pin1 = this->led_arr[0];
    // led_pin2 = this->led_arr[1];
    // led_pin3 = this->led_arr[2];

    // Serial.printf("led_pin1 = %d\nled_pin2 = %d\nled_pin3 = %d",led_pin1,led_pin2,led_pin3),

    
    Serial.write(ch);

    this->print_data();
}
/// @brief 
floor_way::~floor_way()
{
}





TaskHandle_t hc_led_handle1;
TaskHandle_t hc_led_handle2;
TaskHandle_t hc_led_handle3;

// 多线程 三层的 人体传感器控制LED 函数
void HC_LED1(void *pvParameters);
void HC_LED2(void *pvParameters);
void HC_LED3(void *pvParameters);
// led 初始化 
void init_led();
// hc_SR501 人体传感器 set_pin
void init_hc_SR501();
//多线程 物联网云平台
void MQTT_task(void *pvParameters);


void setup() {

    // 初始化
    init_led();
    init_hc_SR501();


    Serial.begin(115200);
    Serial.println("串口开始输出》》》》");

    // // 实例化三层楼的对象
    // Serial.println("实例化楼道灯的类对象");
    // floor_way floor1(fool_led[0], hbs_1, 0); // 假设 fool_led[0] 是第一层楼的 LED 数组
    // floor_way floor2(fool_led[1], hbs_2, 0); // 假设 fool_led[1] 是第二层楼的 LED 数组
    // floor_way floor3(fool_led[2], hbs_3, 0); // 假设 fool_led[2] 是第三层楼的 LED 数组

    // xTaskCreate(HC_LED1, "HC_LED", 6666, (void*)&floor1, 1, &hc_led_handle1);
    // xTaskCreate(HC_LED2, "HC_LED", 6666, (void*)&floor2, 1, &hc_led_handle2);
    // xTaskCreate(HC_LED3, "HC_LED", 6666, (void*)&floor3, 1, &hc_led_handle3);
     xTaskCreate(HC_LED1, "HC_LED", 6666, NULL, 1, &hc_led_handle1);

    // MQTT 部分
    Serial.println("初始化 mqtt,创建mqtt线程");
    MQTT_INIT();
    xTaskCreate(MQTT_task,"MQTT",6666,NULL,2,NULL);     // MQTT 并行任务

}

    int i = 0;

void loop() {



    Serial.print(" 第");
    Serial.print(i++);
    Serial.println("检查人体传感器的状态");

    

    Serial.println(digitalRead(34));
    Serial.println(digitalRead(35));
    Serial.println(digitalRead(32));


    delay(1000);
}






// 三层楼的人体传感器控制 led 的多线程函数
// 亮灯的延时时间计时器
unsigned long timer_floor1 = millis();
unsigned long timer_floor2 = millis();
unsigned long timer_floor3 = millis();
unsigned long timer_chixu = 60000;
void HC_LED1(void *pvParameters) {
    Serial.printf("多线程 楼层1 人体传感器控制led函数\n");
    while (1)
    {
        //floor_way* floor1 = (floor_way*)pvParameters; // 假设第一个参数是 floor1

        if (digitalRead(hbs_1) == 0)
        {
            //floor1->set_led_hgih();
            digitalWrite(fool_led[0][0],HIGH);
            digitalWrite(fool_led[0][1],HIGH);
            digitalWrite(fool_led[0][2],HIGH);
            Serial.println("刷新一号楼的计时器");
            timer_floor1 = millis();
        }
        else if (digitalRead(hbs_1) == 1 && (millis() - timer_floor1) > timer_chixu)
        {
            //floor1->set_led_low();
            digitalWrite(fool_led[0][0],LOW);
            digitalWrite(fool_led[0][1],LOW);                
            digitalWrite(fool_led[0][2],LOW);
            timer_floor1 = millis();
        }

        if (digitalRead(hbs_2) == 0)
        {
            //floor2->set_led_hgih();
            digitalWrite(fool_led[1][0],HIGH);
            digitalWrite(fool_led[1][1],HIGH);
            digitalWrite(fool_led[1][2],HIGH);
            Serial.println("刷新二号楼的计时器");
            timer_floor2 = millis();            
        }
        else if (digitalRead(hbs_2) == 1 && (millis() - timer_floor2) > timer_chixu)
        {
            //floor2->set_led_low();
            digitalWrite(fool_led[1][0],LOW);                
            digitalWrite(fool_led[1][1],LOW);                
            digitalWrite(fool_led[1][2],LOW);      
            timer_floor2 = millis();          
        }

        if (digitalRead(hbs_3) == 0)
        {
            //floor3->set_led_hgih();
            digitalWrite(fool_led[2][0],HIGH);
            digitalWrite(fool_led[2][1],HIGH);
            digitalWrite(fool_led[2][2],HIGH);
            Serial.println("刷新三号楼的计时器");
            timer_floor3 = millis();            
        }
        else if (digitalRead(hbs_3) == 1 && (millis() - timer_floor3) > timer_chixu)
        {
            //floor3->set_led_low();
            digitalWrite(fool_led[2][0],LOW);                
            digitalWrite(fool_led[2][1],LOW);                
            digitalWrite(fool_led[2][2],LOW);                
            timer_floor3 = millis();
        }


        vTaskDelay(1000);
    }
}


// led 初始化 
void init_led()
{
    // 一楼楼道 灯
    pinMode(2, OUTPUT);
    pinMode(15, OUTPUT);
    pinMode(4, OUTPUT);
    // 二楼楼道 灯
    pinMode(16, OUTPUT);
    pinMode(17, OUTPUT);
    pinMode(5, OUTPUT);
    // 三楼楼道 灯
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
}

// hc_SR501 人体传感器 set_pin
void init_hc_SR501(){
    // 人体传感器 一楼
    pinMode(hbs_1, INPUT);
    // 人体传感器 二楼
    pinMode(hbs_2, INPUT);
    // 人体传感器 三楼
    pinMode(hbs_3, INPUT);
}

//mqtt 线程 信息上报任务
void MQTT_task(void *pvParameters){
    while (1)
    {
        MQTT_loop();
        vTaskDelay(1000);
    }
    
}