#ifndef RELAYCONTROL_H
#define RELAYCONTROL_H

#include <Arduino.h>

class RelayControl {
public:
    RelayControl(int pin);

    void on();
    void off();
    void setHoldTime(unsigned long holdTime); // 设置状态保持时间
    void setSwitchDelay(unsigned long switchDelay); // 设置开关延时
    bool getState(); // 获取当前状态

private:
    int PIN;
    bool state; // 记录当前状态
    unsigned long holdTime; // 状态保持时间
    unsigned long switchDelay; // 开关延时
};

#endif // RELAYCONTROL_H
