// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once

#include <Arduino.h>

enum TouchState
{
    TOUCH_NOP,     // nothing happening
    TOUCH_PENDING, // something happening
    TOUCH_TAP,     // completed tap
    TOUCH_HOLD,
};

class TouchSensor
{
private:
    int8_t tapCount = 0;
    uint8_t pin = 44;
    int pinState = LOW;
    uint64_t lastChange = 0L;
    TouchState action = TOUCH_NOP;
    uint16_t threshold = 0; //ms
    uint16_t presshold = 500; //ms

public:
    TouchSensor(uint8_t pin);
    
    ~TouchSensor();

    void setup();
    void tick();
    TouchState GetState();
};

#endif
