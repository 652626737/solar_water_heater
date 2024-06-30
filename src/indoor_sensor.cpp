#include <Arduino.h>
#include <ntc_config.h>
#include "indoor_sensor.h"

// NTC热敏电阻相关参数
const float Vin = 3.3;
const float R_fixed = 10000.0; // 固定电阻的阻值，单位为欧姆 (10KΩ) 建议使用高精度

IndoorSensor::IndoorSensor(int adcPin)
{
    this->adcPin = adcPin;
}

float IndoorSensor::readTemperature()
{
    // 读取ADC值
    int adcValue = analogRead(adcPin);

    // 计算电压值
    float voltage = adcValue * (Vin / 4095.0); // ESP32的ADC分辨率为12位，即4096个步进

    // 计算NTC热敏电阻的阻值
    float Rntc = calculateResistance(voltage);

    // 根据斯坦哈特方程计算温度
    float temperature = calculateTemperature(Rntc);

    return temperature;
}

float IndoorSensor::calculateResistance(float voltage)
{
    // 计算NTC热敏电阻的阻值
    float Rntc = R_fixed * (Vin / voltage - 1.0);
    return Rntc;
}

float IndoorSensor::calculateTemperature(float resistance)
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
