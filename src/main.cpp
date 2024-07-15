#include <Arduino.h>
#include "sensors_and_monitor.h"
#include "WiFiSTAClient.h"
#include <ArduinoJson.h>

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
const TickType_t senddataDelay = pdMS_TO_TICKS(2000);
#else
// 定义任务执行间隔时间（单位：毫秒）
const TickType_t ROOM_SENSOR_INTERVAL = 300000 / portTICK_PERIOD_MS;         // 每5分钟行一次
const TickType_t ANTIFREEZE_SENSOR_INTERVAL = 300000 / portTICK_PERIOD_MS;   // 每5分钟行一次
const TickType_t WATERHEATER_SENSOR_INTERVAL = 300000 / portTICK_PERIOD_MS;  // 每5分钟行一次
const TickType_t WATER_LEVEL_MONITOR_INTERVAL = 300000 / portTICK_PERIOD_MS; // 每5分钟行一次
const TickType_t checkWaterLevelDelay = pdMS_TO_TICKS(3600000);              // 每1小时执行一次
const TickType_t addWaterDelay = pdMS_TO_TICKS(7200000);
const TickType_t senddataDelay = pdMS_TO_TICKS(5000);                     // 每2小时执行一次
#endif

const char *sta_ssid = "ESP32_AP";
const char *sta_password = "12345678";
const char *sta_server_ip = "192.168.4.1";
const int sta_server_port = 80;

WiFiSTAClient staClient(sta_ssid, sta_password, sta_server_ip, sta_server_port);


void tasksenddata(void *pvParameters)
{

    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        // 如果连接到Wi-Fi并且连接到服务器成功，则发送数据
        if (staClient.isAPConnected() && staClient.connectToServer())
        {
            // 构建JSON对象

            StaticJsonDocument<200> jsonDoc; // 可以根据需要调整JSON文档的大小
            
            
            jsonDoc["indoorSensor"] = sensorData.indoorSensorNumder;
            jsonDoc["outdoorSensor"] = sensorData.outdoorSensorNumder;
            jsonDoc["waterheaterSensor"] = sensorData.waterheaterSensorNumder;
            jsonDoc["waterLevelMonitor"] = sensorData.waterLevelNumber;
        
            // 序列化JSON为字符串
            String jsonString;
            serializeJson(jsonDoc, jsonString);

            // 向服务器发送JSON数据
            staClient.sendData(jsonString);
            Serial.println(staClient.receiveData());
        }

        // 延迟一段时间后再次发送数据（示例中为2秒）
        vTaskDelay(interval);
    }
}


void setup()
{
    Serial.begin(115200);
    staClient.connectToAP();
    if (staClient.isAPConnected())
    {
        Serial.println("WiFi connected");
        if (staClient.connectToServer())
        {
            Serial.println("Connected to server");
        };
    }

    Serial.println("Starting up...");

    pinMode(WORK_MOS_PIN, OUTPUT);

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
    xTaskCreate(tasksenddata, "SendDataTask", 2048, (void *)&senddataDelay, 1, NULL);
}

void loop()
{
    delay(2000);
}
