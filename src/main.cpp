#include <Arduino.h>
#include <ArduinoJson.h>
#include "sensors_and_monitor.h"
#include <ESPAsyncWebServer.h>

// 定义一个debug标记
// #define DEBUG_FLAG
#ifdef DEBUG_FLAG
// 定义任务执行间隔时间（单位：毫秒）
const TickType_t ROOM_SENSOR_INTERVAL = 3000 / portTICK_PERIOD_MS;         // 每5秒执行一次
const TickType_t ANTIFREEZE_SENSOR_INTERVAL = 3000 / portTICK_PERIOD_MS;   // 每5秒执行一次
const TickType_t WATERHEATER_SENSOR_INTERVAL = 3000 / portTICK_PERIOD_MS;  // 每5秒执行一次
const TickType_t WATER_LEVEL_MONITOR_INTERVAL = 3000 / portTICK_PERIOD_MS; // 每5秒执行一次
const TickType_t checkWaterLevelDelay = pdMS_TO_TICKS(3000);               // 每3秒执行一次
const TickType_t addWaterDelay = pdMS_TO_TICKS(10000);                      // 每5秒执行一次
#else
// 定义任务执行间隔时间（单位：毫秒）
const TickType_t ROOM_SENSOR_INTERVAL = 60000 / portTICK_PERIOD_MS;         // 每1分钟行一次
const TickType_t ANTIFREEZE_SENSOR_INTERVAL = 60000 / portTICK_PERIOD_MS;   // 每1分钟行一次
const TickType_t WATERHEATER_SENSOR_INTERVAL = 60000 / portTICK_PERIOD_MS;  // 每1分钟行一次
const TickType_t WATER_LEVEL_MONITOR_INTERVAL = 60000 / portTICK_PERIOD_MS; // 每1分钟行一次
const TickType_t checkWaterLevelDelay = pdMS_TO_TICKS(60000);              // 每1分钟执行一次
const TickType_t addWaterDelay = pdMS_TO_TICKS(7200000);
#endif

// WiFi接入点（AP）参数
const char *ap_ssid = "ESP32_AP";
const char *ap_password = "12345678";

AsyncWebServer server(80);

unsigned long lastHeartbeat = 0;
const unsigned long heartbeatInterval = 10000; // 10秒


void setup()
{
    Serial.begin(115200);

    // 启动AP模式
    WiFi.softAP(ap_ssid, ap_password);
    Serial.println("Access Point Started");

    // 打印AP的IP地址
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { 
                StaticJsonDocument<200> jsonDoc; // 可以根据需要调整JSON文档的大小
                jsonDoc["indoorSensor"] = sensorData.indoorSensorNumder;
                jsonDoc["outdoorSensor"] = sensorData.outdoorSensorNumder;
                jsonDoc["waterheaterSensor"] = sensorData.waterheaterSensorNumder;
                jsonDoc["waterLevelMonitor"] = sensorData.waterLevelNumber;

                // 序列化JSON为字符串
                String jsonString;
                serializeJson(jsonDoc, jsonString);
                
                request->send(200, "application/json", jsonString); });

    server.begin();

    pinMode(WORK_MOS_PIN, OUTPUT);
    pinMode(WIFI_LED, OUTPUT);
    pinMode(STATUS_LED, OUTPUT);

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
}