// Copyright (c) 2021 Dave Marsh. See LICENSE.

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
    uint64_t now = millis();
    int currentState = digitalRead(pin);
    if (currentState == pinState)
    {
        return;
    }

    int64_t timediff = now - lastChange;
    if (timediff < threshold)
    {
        return;
    }

    pinState = currentState;
    lastChange = now;
    if (pinState == HIGH)
    {
        action = TOUCH_PENDING;
        return;
    }

    if (timediff > presshold)
    {
        action = TOUCH_HOLD;
    }
    else 
    {
        action = TOUCH_TAP;
    }
    lastChange = now;
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