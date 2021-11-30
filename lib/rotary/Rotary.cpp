// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#include "Rotary.h"

Rotary::Rotary(uint8_t c,
               uint8_t d,
               uint8_t b) : clockPin(c), dataPin(d), buttonPin(b)
{
}

Rotary::~Rotary()
{
}

void Rotary::setup()
{
    pinMode(clockPin, INPUT);
    pinMode(dataPin, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);
    lastClockState = digitalRead(clockPin);
}

void Rotary::loop()
{
    uint64_t now = millis();
    if (now < lastTime + 1)
    {
        return;
    }

    int clockState = digitalRead(clockPin);

    if (clockState == HIGH && lastClockState == LOW)
    {
        int dataState = digitalRead(dataPin);
        if (dataState == clockState)
        {
            counter--;
            state = ROTARY_COUNTER_CLOCKWISE;
        }
        else
        {
            counter++;
            state = ROTARY_CLOCKWISE;
        }
    }
    lastClockState = clockState;

    if (state == ROTARY_NOP)
    {
        int buttonState = digitalRead(buttonPin);
        if (buttonState == LOW && lastButtonState != LOW)
        {
            lastButtonClick = now;
            state = ROTARY_CLICK;
        }
        lastButtonState = buttonState;
    }

    lastTime = now;
}

RotaryState Rotary::GetState()
{
    RotaryState s = state;
    state = ROTARY_NOP;
    return s;
}

#endif
