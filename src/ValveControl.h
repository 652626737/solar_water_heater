#ifndef VALVECONTROL_H
#define VALVECONTROL_H

#include <Arduino.h>

class ValveControl {
public:
    ValveControl(int pin);

    void open();
    void close();

private:
    int PIN;
};

#endif // VALVECONTROL_H
