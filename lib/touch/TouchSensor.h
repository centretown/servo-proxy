// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>

enum ActionState
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
    ActionState action = TOUCH_NOP;
    uint16_t threshold = 400;//ms
    uint16_t presshold = 700;//ms

public:
    TouchSensor(uint8_t pin);
    ~TouchSensor();

    void setup();
    void loop();
    ActionState getState();
};
