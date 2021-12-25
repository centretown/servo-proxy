// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#include "TouchSensor.h"

TouchSensor::TouchSensor(uint8_t pin) : pin(pin)
{
}

TouchSensor::~TouchSensor()
{
}

void TouchSensor::setup()
{
    pinMode(pin, INPUT);
}

void TouchSensor::tick()
{
    int currentState = digitalRead(pin);
    if (currentState == pinState)
    {
        return;
    }

    uint64_t now = millis();
    uint64_t timediff = now - lastChange;
    lastChange = now;
    pinState = currentState;
    if (pinState == HIGH)
    {
        action = TOUCH_PENDING;
        return;
    }

    action = (timediff > presshold) ? TOUCH_HOLD : TOUCH_TAP;
}

TouchState TouchSensor::GetState()
{
    TouchState state = action;
    if (state > TOUCH_PENDING)
    {
        action = TOUCH_NOP;
    }
    return state;
}

#endif
