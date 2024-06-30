#include <Arduino.h>
#include <ntc_config.h>
#include "waterheater_sensor.h"

// NTC热敏电阻相关参数
const float Vin = 12.00;        // 输入电压
const float R_fixed1 = 10000.0; // 第一级分压电阻
const float R_fixed2 = 30000.0; // 第二级分压电阻 R2
const float R_fixed3 = 10000.0; // 第二级分压电阻 R3

WaterheaterSensor::WaterheaterSensor(int adcPin)
{
    this->adcPin = adcPin;
}

float WaterheaterSensor::readTemperature()
{

    // 计算第二级分压后的电压值
    float voltage = analogReadMilliVolts(adcPin) / 1000.00;
    Serial.print("voltage = ");
    Serial.println(voltage);

    // 计算NTC热敏电阻的阻值
    float Rntc = calculateResistance(voltage);

    // 根据斯坦哈特方程计算温度
    float temperature = calculateTemperature(Rntc);

    return temperature;
}

float WaterheaterSensor::calculateResistance(float voltage)
{
    // 计算第一级分压后的电压值
    float V1 = voltage * ((R_fixed2 + R_fixed3) / R_fixed3);
    // 计算NTC热敏电阻的阻值
    float R_ntc = R_fixed1 * (Vin / V1 - 1);
    return R_ntc;
}

float WaterheaterSensor::calculateTemperature(float resistance)
{
    const float A = 1.009249522e-03;
    const float B = 2.378405444e-04;
    const float C = 2.019202697e-07;

    float logR = log(resistance);
    float steinhart = A + B * logR + C * logR * logR * logR;
    steinhart = 1.0 / steinhart; // Invert
    steinhart -= 273.15;         // Convert to Celsius
    return steinhart;
}
