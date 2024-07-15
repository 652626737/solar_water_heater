#include "RelayControl.h"

RelayControl::RelayControl(int pin) {
    PIN = pin;
    pinMode(PIN, OUTPUT);
    state = LOW; // 默认关闭状态
    holdTime = 0;
    switchDelay = 0;
}

void RelayControl::on() {
    digitalWrite(PIN, HIGH);
    state = HIGH;
    if (holdTime > 0) {
        delay(holdTime);
        digitalWrite(PIN, LOW); // 状态保持时间结束后自动关闭
        state = LOW;
    }
}

void RelayControl::off() {
    if (switchDelay > 0) {
        delay(switchDelay);
    }
    digitalWrite(PIN, LOW);
    state = LOW;
}

void RelayControl::setHoldTime(unsigned long holdTime) {
    this->holdTime = holdTime;
}

void RelayControl::setSwitchDelay(unsigned long switchDelay) {
    this->switchDelay = switchDelay;
}

bool RelayControl::getState() {
    return state;
}
