#ifndef WATERHEATER_SENSOR_H
#define WATERHEATER_SENSOR_H

class WaterheaterSensor
{
public:
    WaterheaterSensor(int adcPin);
    float readTemperature();

private:
    int adcPin;
    float calculateResistance(float voltage);
    float calculateTemperature(float resistance);
};

#endif // WATERHEATER_SENSOR_H
