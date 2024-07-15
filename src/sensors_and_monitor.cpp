#include "sensors_and_monitor.h"

IndoorSensor indoorSensor(R0_ROOM_ADC_PIN);
OutdoorSensor outdoorsensor(R0_ANTIFREEZE_ADC_PIN);
WaterheaterSensor waterheaterSensor(R0_WATERHEATER_ADC_PIN);
WaterLevelMonitor waterLevelMonitor(WATER_LEVEL_ADC_PIN);


// 信号量
SemaphoreHandle_t xMutex;
SemaphoreHandle_t xWaterAddSemaphore;
volatile bool waterAddingInProgress = false; // 全局标志，指示是否正在进行加水

SensorData sensorData;

void taskIndoorSensor(void *pvParameters)
{
    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(WORK_MOS_PIN, HIGH);
            vTaskDelay(50);
            sensorData.indoorSensorNumder = indoorSensor.readTemperature();
            Serial.print("Room Temperature: ");
            Serial.println(sensorData.indoorSensorNumder);
            digitalWrite(WORK_MOS_PIN, LOW);
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(interval);
    }
}

void taskOutdoorSensor(void *pvParameters)
{
    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(WORK_MOS_PIN, HIGH);
            sensorData.outdoorSensorNumder = outdoorsensor.readTemperature();
            Serial.print("Antifreeze Temperature: ");
            Serial.println(sensorData.outdoorSensorNumder);
            digitalWrite(WORK_MOS_PIN, LOW);
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(interval);
    }
}

void taskWaterheaterSensor(void *pvParameters)
{
    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(WORK_MOS_PIN, HIGH);
            vTaskDelay(50);
            sensorData.waterheaterSensorNumder = waterheaterSensor.readTemperature();
            Serial.print("Water Heater Temperature: ");
            Serial.println(sensorData.waterheaterSensorNumder);
            digitalWrite(WORK_MOS_PIN, LOW);
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(interval);
    }
}

void taskWaterLevelMonitor(void *pvParameters)
{
    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(WORK_MOS_PIN, HIGH);
            vTaskDelay(50);
            sensorData.waterheaterSensorNumder = waterLevelMonitor.update();
            Serial.print("Water Level: ");
            Serial.println(sensorData.waterheaterSensorNumder);
            digitalWrite(WORK_MOS_PIN, LOW);
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(interval);
    }
}

void taskCheckWaterLevel(void *pvParameters)
{
    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            digitalWrite(WORK_MOS_PIN, HIGH);
            vTaskDelay(50);
            float waterLevel = waterLevelMonitor.update();
            Serial.print("Water Level: ");
            Serial.println(waterLevel);
            if (waterLevel <= 80.00 && !waterAddingInProgress)
            {
                Serial.println("Water level below 80%. Notifying taskAddWater...");
                waterAddingInProgress = true;       // 设置标志
                xSemaphoreGive(xWaterAddSemaphore); // Notify taskAddWater to add water
            }
            digitalWrite(WORK_MOS_PIN, LOW);
            xSemaphoreGive(xMutex);
        }
        vTaskDelay(interval);
    }
}



void taskAddWater(void *pvParameters)
{
    TickType_t interval = *(TickType_t *)pvParameters;
    while (true)
    {
        if (xSemaphoreTake(xWaterAddSemaphore, portMAX_DELAY) == pdTRUE && xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE)
        {
            vTaskDelay(interval);
            digitalWrite(WORK_MOS_PIN, HIGH);
            Serial.println("Adding water...");
            waterLevelMonitor.addWater(); // 假设 waterLevelMonitor.addWater() 是加水函数
            digitalWrite(WORK_MOS_PIN, LOW);
            Serial.println("Water addition complete.");
            // 加水完成后，重置标志
            waterAddingInProgress = false;
        }
        xSemaphoreGive(xMutex);
    }
}


bool selfTest()
{
    bool result = true;

    digitalWrite(WORK_MOS_PIN, HIGH);

    float roomTemp = indoorSensor.readTemperature();
    if (isnan(roomTemp))
    {
        Serial.println("Room sensor self-test failed!");
        result = false;
    }
    else
    {
        Serial.print("Room sensor self-test passed. Temperature: ");
        Serial.println(roomTemp);
    }

    float antifreezeTemp = outdoorsensor.readTemperature();
    if (isnan(antifreezeTemp))
    {
        Serial.println("Antifreeze sensor self-test failed!");
        result = false;
    }
    else
    {
        Serial.print("Antifreeze sensor self-test passed. Temperature: ");
        Serial.println(antifreezeTemp);
    }

    float waterheaterTemp = waterheaterSensor.readTemperature();
    if (isnan(waterheaterTemp))
    {
        Serial.println("Water heater sensor self-test failed!");
        result = false;
    }
    else
    {
        Serial.print("Water heater sensor self-test passed. Temperature: ");
        Serial.println(waterheaterTemp);
    }

    float waterLevel = waterLevelMonitor.update();
    if (isnan(waterLevel))
    {
        Serial.println("Water level monitor self-test failed!");
        result = false;
    }
    else
    {
        Serial.print("Water level monitor self-test passed. Level: ");
        Serial.println(waterLevel);
    }

    digitalWrite(WORK_MOS_PIN, LOW);

    return result;
}
