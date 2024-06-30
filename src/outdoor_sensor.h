#ifndef OUTDOOR_SENSOR_H
#define OUTDOOR_SENSOR_H

class OutdoorSensor
{
public:
    OutdoorSensor(int adcPin);
    float readTemperature();

private:
    int adcPin;
    float calculateResistance(float voltage);
    float calculateTemperature(float resistance);
};

#endif // OUTDOOR_SENSOR_H