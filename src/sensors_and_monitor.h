#ifndef SENSORS_AND_MONITOR_H
#define SENSORS_AND_MONITOR_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "ntc_config.h"
#include "indoor_sensor.h"
#include "outdoor_sensor.h"
#include "waterheater_sensor.h"
#include "water_level_monitor.h"


typedef struct {
    float indoorSensorNumder;
    float outdoorSensorNumder;
    float waterheaterSensorNumder;
    float waterLevelNumber;
} SensorData;



// 传感器实例化
extern IndoorSensor indoorSensor;
extern OutdoorSensor outdoorSensor;
extern WaterheaterSensor waterheaterSensor;
extern WaterLevelMonitor waterLevelMonitor;

// 互斥锁
extern SemaphoreHandle_t xMutex;

// 二进制信号量
extern SemaphoreHandle_t xWaterAddSemaphore;

extern SensorData sensorData;

// 传感器任务声明
void taskIndoorSensor(void *pvParameters);
void taskOutdoorSensor(void *pvParameters);
void taskWaterheaterSensor(void *pvParameters);
void taskWaterLevelMonitor(void *pvParameters);

// 加水任务声明
void taskCheckWaterLevel(void *pvParameters);
void taskAddWater(void *pvParameters);


// 自检函数声明
bool selfTest();

#endif // SENSORS_AND_MONITOR_H
