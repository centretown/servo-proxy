// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "EndPoint.h"

bool EndPoint::process(UserEvent event)
{
    if (event == USER_SELECT)
    {
        return false;
    }
    int16_t direction = (event == USER_PREVIOUS) ? -1 : 1;
    int64_t now = millis();
    int64_t diff = now - last;
    last = now;
    if (diff < 200)
    {
        int16_t factor = 200 / diff;
        direction *= (1 << factor);
    }
    SetCounter(GetCounter() + direction);
    return true;
}

void EndPoint::setup(uint8_t i, uint8_t c, uint8_t p)
{
    index = i;
    command = c;
    parameter = p;
}
