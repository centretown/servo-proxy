// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#include "Rotary.h"

Rotary::Rotary(RotaryEncoder &enc,
               uint8_t b) : enc(enc), buttonPin(b)
{
}

Rotary::~Rotary()
{
}

void Rotary::setup()
{
    pinMode(buttonPin, INPUT_PULLUP);
}

void Rotary::loop()
{
    uint64_t now = millis();
    if (now < lastTime + 2)
    {
        return;
    }

    enc.tick();
    long newPos = enc.getPosition();
    if (lastPos > newPos)
    {
        state = ROTARY_COUNTER_CLOCKWISE;
        counter--;
    }
    else if (lastPos < newPos)
    {
        state = ROTARY_CLOCKWISE;
        counter++;
    }
    else
    {
        int buttonState = digitalRead(buttonPin);
        if (buttonState == LOW && lastButtonState != LOW)
        {
            state = ROTARY_CLICK;
        }
        lastButtonState = buttonState;
    }

    lastPos = newPos;
    lastTime = now;
}

RotaryState Rotary::GetState()
{
    RotaryState s = state;
    state = ROTARY_NOP;
    return s;
}

#endif
