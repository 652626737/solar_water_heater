#include "WiFiConnection.h"

// 连接到 WiFi AP 的函数定义
void connectToAP(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.print("Connected to WiFi. IP address: ");
    Serial.println(WiFi.localIP());
}

// 启动热点模式的函数定义
void startAP(const char *ssid, const char *password)
{
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

// 获取信号强度的函数实现
int32_t getSignalStrength() {
    return WiFi.RSSI();
}


// 检查 WiFi 连接状态的函数定义
bool isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

// 断开 WiFi 连接的函数定义
void disconnectWiFi()
{
    WiFi.disconnect();
    Serial.println("Disconnected from WiFi.");
}
