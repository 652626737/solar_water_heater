#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <WiFi.h>

// 声明连接到 WiFi AP 的函数
void connectToAP(const char *ssid, const char *password);

// 声明启动热点模式的函数
void startAP(const char *ssid, const char *password);

int32_t getSignalStrength(); // 获取信号强度的函数声明

// 声明检查 WiFi 连接状态的函数
bool isConnected();

// 声明断开 WiFi 连接的函数
void disconnectWiFi();

#endif // WIFICONNECTION_H
