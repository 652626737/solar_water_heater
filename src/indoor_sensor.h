#ifndef INDOOR_SENSOR_H
#define INDOOR_SENSOR_H

class IndoorSensor
{
public:
    IndoorSensor(int adcPin);
    float readTemperature();

private:
    int adcPin;
    float calculateResistance(float voltage);
    float calculateTemperature(float resistance);
};

#endif // INDOOR_SENSOR_H
