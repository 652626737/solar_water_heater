#ifndef NTC_CONFIG_H
#define NTC_CONFIG_H

// MCU引脚配置
const int R0_ROOM_ADC_PIN = 34;        // 房间NTC热敏电阻连接的模拟输入引脚
const int R0_ANTIFREEZE_ADC_PIN = 35;  // 防冻NTC热敏电阻连接的模拟输入引脚
const int R0_WATERHEATER_ADC_PIN = 33; // 热水NTC热敏电阻连接的模拟输入引脚
const int WATER_LEVEL_ADC_PIN = 32;    // 水位传感器连接的模拟输入引脚
const int WORK_MOS_PIN = 18;           // 水位传感器检测到水位的LED引脚
const int RELAY_PIN_1 = 14;            // 继电器引脚
const int RELAY_PIN_2 = 12;            // 继电器引脚

const int R0_room = 1000.00;

// 防冻NTC热敏电阻的参数
const float r0_antifreeze = 10000.0;  // 在参考温度下的电阻值，单位为欧姆
const float beta_antifreeze = 3950.0; // Beta系数，通常为3950
const float t0_antifreeze = 25.0;     // 参考温度，单位为摄氏度

// 水位监测配置
const float ADC_100_PERCENT = 2800.00; // 100%水位对应的ADC步进值
const float ADC_80_PERCENT = 1100.00;  // 80%水位对应的ADC步进值
const float ADC_50_PERCENT = 700.00;   // 50%水位对应的ADC步进值
const float ADC_20_PERCENT = 500.00;   // 20%水位对应的ADC步进值

// 加水配置
const unsigned long MAX_ADD_WATER_TIME_MS = 1200000; // 最长加水时间为20分钟，单位为毫秒

// 温度补偿
const float R0_room_compensation = 1.0;        // 房间NTC热敏电阻温度补偿系数
const float R0_antifreeze_compensation = 3.0;  // 防冻NTC热敏电阻温度补偿系数
const float R0_waterheater_compensation = 2.0; // 热水NTC热敏电阻温度补偿系数



#endif // NTC_CONFIG_H
