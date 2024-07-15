#include "ValveControl.h"

ValveControl::ValveControl(int pin) {
    PIN = pin;
    pinMode(PIN, OUTPUT);
}

void ValveControl::open() {
    digitalWrite(PIN, HIGH);
}

void ValveControl::close() {
    digitalWrite(PIN, LOW);
}
