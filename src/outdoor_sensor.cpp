#include <Arduino.h>
#include <ntc_config.h>
#include "outdoor_sensor.h"

// NTC热敏电阻相关参数
const float Vin = 12.00;        // 输入电压
const float R_fixed1 = 10000.0; // 第一级分压电阻
const float R_fixed2 = 30000.0; // 第二级分压电阻 R2
const float R_fixed3 = 10000.0; // 第二级分压电阻 R3

OutdoorSensor::OutdoorSensor(int adcPin)
{
    this->adcPin = adcPin;
}

float OutdoorSensor::readTemperature()
{
    // 计算第二级分压后的电压值
    float voltage = analogReadMilliVolts(adcPin) / 1000.00;
    Serial.print("voltage = ");
    Serial.println(voltage);

    // 计算NTC热敏电阻的阻值
    float Rntc = calculateResistance(voltage);

    // 根据斯坦哈特方程计算温度
    float temperature = calculateTemperature(Rntc)+R0_antifreeze_compensation;

    return temperature;
}

float OutdoorSensor::calculateResistance(float voltage)
{
    // 计算第一级分压后的电压值
    float V1 = voltage * ((R_fixed2 + R_fixed3) / R_fixed3);
    // 计算NTC热敏电阻的阻值
    float R_ntc = R_fixed1 * (Vin / V1 - 1);
    return R_ntc;
}

float OutdoorSensor::calculateTemperature(float resistance) {
    // 根据斯坦哈特方程计算温度
    float steinhart;
    steinhart = resistance / r0_antifreeze;         // (R/R0)
    steinhart = log(steinhart);          // ln(R/R0)
    steinhart /= beta_antifreeze;                   // 1/B * ln(R/R0)
    steinhart += 1.0 / (t0_antifreeze + 273.15);    // + (1/To)
    steinhart = 1.0 / steinhart;         // Invert
    steinhart -= 273.15;                 // Convert to Celsius
    return steinhart;
}