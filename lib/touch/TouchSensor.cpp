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
    Serial.print("Touch Pin");
    Serial.println(pin);
}

void TouchSensor::loop()
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

ActionState TouchSensor::getState()
{
    ActionState state = action;
    if (state > TOUCH_PENDING)
    {
        action = TOUCH_NOP;
    }
    return state;
}

#endif
