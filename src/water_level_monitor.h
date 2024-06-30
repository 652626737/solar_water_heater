#ifndef WATER_LEVEL_MONITOR_H
#define WATER_LEVEL_MONITOR_H

#include "KalmanFilter.h"

class WaterLevelMonitor
{
public:
    WaterLevelMonitor(int adcPin);
    float update();
    void addWater();

private:
    int adcPin;
    float readADCValue();
    float calculateWaterLevelPercentage(float adcValue);
    KalmanFilter kalmanFilter;
};

#endif // WATER_LEVEL_MONITOR_H
