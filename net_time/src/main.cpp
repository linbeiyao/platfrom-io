#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "123456";    //你的网络名称
const char *password = "88888888"; //你的网络密码

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 8 * 3600;
const int daylightOffset_sec = 0;

void printLocalTime()
{
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.println(&timeinfo, "%F %T %A"); // 格式化输出
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("WiFi connected!");
    Serial.print("WIFI 地址：");
    Serial.println(WiFi.localIP());

    // 从网络时间服务器上获取并设置时间
    // 获取成功后芯片会使用RTC时钟保持时间的更新
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();

    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP_STA);
    Serial.println("WiFi disconnected!");
}

void loop()
{
    delay(1000);
    printLocalTime();
}
