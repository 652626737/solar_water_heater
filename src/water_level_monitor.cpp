#include "water_level_monitor.h"
#include <Arduino.h>
#include "ntc_config.h"

WaterLevelMonitor::WaterLevelMonitor(int adcPin) : adcPin(adcPin), kalmanFilter(2, 0.5, 1023, 0)
{
}

float WaterLevelMonitor::update()
{   float waterLevelPercentage = 0.00;
    // 读取ADC值并计算水位百分比
    float adcValue = readADCValue();
    waterLevelPercentage = calculateWaterLevelPercentage(adcValue);
    return waterLevelPercentage;
}

float WaterLevelMonitor::readADCValue()
{
    float adcValue = analogRead(adcPin);
    float filteredValue = kalmanFilter.update(adcValue);

    // Serial.print("ADC Value: ");
    // Serial.print(adcValue);
    Serial.print(" Filtered Value: ");
    Serial.println(filteredValue);

    return filteredValue;
}

/**
 * @brief 线性插值
 *
 * 根据给定的两个点 (x0, y0) 和 (x1, y1)，使用线性插值计算给定 x 值对应的 y 值。
 *
 * @param x 给定的 x 值
 * @param x0 第一个点的 x 坐标
 * @param x1 第二个点的 x 坐标
 * @param y0 第一个点的 y 坐标
 * @param y1 第二个点的 y 坐标
 *
 * @return 给定 x 值对应的 y 值
 */
float linearInterpolation(int x, int x0, int x1, float y0, float y1)
{
    return y0 + ((float)(x - x0) * (y1 - y0)) / (x1 - x0);
}

// 水位检测函数
/**
 * @brief 获取水位百分比
 *
 * 根据 ADC 值计算水位百分比。
 *
 * @param adcValue ADC 值
 *
 * @return 返回水位百分比，范围为 0.0 到 100.0
 */
float WaterLevelMonitor::calculateWaterLevelPercentage(float adcValue) {
    // 根据ADC值计算水位百分比
    if (adcValue<ADC_20_PERCENT) {
        return 0.0;
    } else if (adcValue<ADC_50_PERCENT && adcValue>=ADC_20_PERCENT) {
        return 20.0;
    } else if (adcValue<ADC_80_PERCENT && adcValue>=ADC_50_PERCENT) {
        return 50.0;
    } else if (adcValue<ADC_100_PERCENT&& adcValue>=ADC_80_PERCENT) {
        return 80.0;
    } else if (adcValue>ADC_100_PERCENT) {
        return 100.0;
    } else {
        return -1;  // 如果ADC值不在任何范围内，则水位为0%
    }
}

void WaterLevelMonitor::addWater()
{
    
    unsigned long startTime = millis();                  // 记录加水开始时间

    Serial.println("Adding water...");

    digitalWrite(RELAY_1_PIN, HIGH); // 打开加水设备，示例中假设使用继电器控制加水

    while (true)
    {

        float waterLevelPercentage = update(); // 更新水位百分比
        Serial.println("waterLevelPercentage:");
        Serial.println(waterLevelPercentage);
        // 检查水位是否达到100%
        if (waterLevelPercentage == 100.00)
        {
            Serial.println("Water added successfully.");
            break; // 水位达到100%，结束加水
        }

        // 检查加水时间是否超过最大时间限制
        if (millis() - startTime > MAX_ADD_WATER_TIME_MS)
        {
            Serial.println("Max add water time exceeded. Stopping.");
            break; // 超过最长加水时间，结束加水
        }

        delay(1000); // 延迟1秒钟，继续监测水位
    }

    digitalWrite(RELAY_1_PIN, LOW); // 关闭加水设备
}
