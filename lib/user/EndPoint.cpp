// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "EndPoint.h"

bool EndPoint::process(UserEvent event)
{
    int64_t now = millis();
    int64_t diff = now - last;
    last = now;

    if (diff < 100)
    {
        increment = 10;
    }
    else if (diff < 200)
    {
        increment = 5;
    }
    else if (diff < 500)
    {
        increment = 2;
    }
    else
    {
        increment = 1;
    }

    int16_t counter = GetCounter();
    switch (event)
    {
    case USER_PREVIOUS:
        counter -= increment;
        break;
    case USER_NEXT:
        counter += increment;
        break;
    default:
        return false;
    }
    SetCounter(counter);
    return true;
}

void EndPoint::setup(uint8_t i, uint8_t c, uint8_t p)
{
    index = i;
    command = c;
    parameter = p;
}
