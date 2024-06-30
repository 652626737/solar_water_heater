#include <Arduino.h>
#include "sensors_and_monitor.h"
#include <esp_now.h>
#include <WiFi.h>
#include <WiFiClient.h>

// 定义一个debug标记
#define DEBUG_FLAG 1
#ifdef DEBUG_FLAG
// 定义任务执行间隔时间（单位：毫秒）
const TickType_t ROOM_SENSOR_INTERVAL = 5000 / portTICK_PERIOD_MS;         // 每5秒执行一次
const TickType_t ANTIFREEZE_SENSOR_INTERVAL = 5000 / portTICK_PERIOD_MS;   // 每5秒执行一次
const TickType_t WATERHEATER_SENSOR_INTERVAL = 5000 / portTICK_PERIOD_MS;  // 每5秒执行一次
const TickType_t WATER_LEVEL_MONITOR_INTERVAL = 5000 / portTICK_PERIOD_MS; // 每5秒执行一次
const TickType_t checkWaterLevelDelay = pdMS_TO_TICKS(3000);               // 每3秒执行一次
const TickType_t addWaterDelay = pdMS_TO_TICKS(5000);                      // 每5秒执行一次

#else
// 定义任务执行间隔时间（单位：毫秒）
const TickType_t ROOM_SENSOR_INTERVAL = 300000 / portTICK_PERIOD_MS;         // 每5分钟行一次
const TickType_t ANTIFREEZE_SENSOR_INTERVAL = 300000 / portTICK_PERIOD_MS;   // 每5分钟行一次
const TickType_t WATERHEATER_SENSOR_INTERVAL = 300000 / portTICK_PERIOD_MS;  // 每5分钟行一次
const TickType_t WATER_LEVEL_MONITOR_INTERVAL = 300000 / portTICK_PERIOD_MS; // 每5分钟行一次
const TickType_t checkWaterLevelDelay = pdMS_TO_TICKS(3600000);              // 每1小时执行一次
const TickType_t addWaterDelay = pdMS_TO_TICKS(7200000);                     // 每2小时执行一次
#endif

// AP 的 SSID 和密码
const char *ssid = "ESP32_AP";
const char *password = "12345678";

// 创建一个 WiFiServer 对象
WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    // 设置 ESP32 为 AP 模式
    // WiFi.softAP(ssid, password);

    // // 启动服务器
    // server.begin();

    // Serial.print("AP IP address: ");
    // Serial.println(WiFi.softAPIP());

    Serial.println("Starting up...");

    pinMode(WORK_MOS_PIN, OUTPUT);
    pinMode(RELAY_1_PIN, OUTPUT);
    pinMode(RELAY_2_PIN, OUTPUT);

    // 自检
    // if (!selfTest())
    // {
    //     Serial.println("Self-test failed. Halting system.");
    //     while (true)
    //     {
    //         vTaskDelay(1000 / portTICK_PERIOD_MS); // 延迟1秒钟
    //     }
    // }

    // 创建互斥锁
    xMutex = xSemaphoreCreateMutex();
    xWaterAddSemaphore = xSemaphoreCreateMutex();

    if (xMutex != NULL)
    {
        // 创建任务
        xTaskCreate(taskIndoorSensor, "Room Sensor Task", 2048, (void *)&ROOM_SENSOR_INTERVAL, 1, NULL);
        xTaskCreate(taskOutdoorSensor, "Antifreeze Sensor Task", 2048, (void *)&ANTIFREEZE_SENSOR_INTERVAL, 1, NULL);
        xTaskCreate(taskWaterheaterSensor, "Water Heater Sensor Task", 2048, (void *)&WATERHEATER_SENSOR_INTERVAL, 1, NULL);
        xTaskCreate(taskWaterLevelMonitor, "Water Level Monitor Task", 2048, (void *)&WATER_LEVEL_MONITOR_INTERVAL, 1, NULL);
        xTaskCreate(taskCheckWaterLevel, "Check Water Level Task", 2048, (void *)&checkWaterLevelDelay, 1, NULL);
        xTaskCreate(taskAddWater, "Add Water Task", 2048, (void *)&addWaterDelay, 1, NULL);
    }
    else
    {
        Serial.println("Failed to create mutex");
    }
}

void loop()
{
    // 处理客户端连接
    // WiFiClient client = server.available(); // 检查是否有新的客户端连接

    // if (client)
    // { // 如果有新的客户端连接
    //     Serial.println("New Client Connected.");
    //     if (client.connected())
    //     {
    //         if (client.available())
    //         {                                                  // 如果客户端有数据可读
    //             String request = client.readStringUntil('\r'); // 读取数据
    //             Serial.println(request);
    //             client.flush();

    //             // 回复客户端
    //             client.print("Hello from ESP32 AP");
    //             delay(10);
    //         }
    //     }
    //     client.stop(); // 关闭客户端连接
    //     Serial.println("Client Disconnected.");
    // }
}
