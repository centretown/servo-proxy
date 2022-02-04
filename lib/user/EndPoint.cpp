// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "EndPoint.h"

bool EndPoint::Process(UserEvent event)
{
    if (event == USER_SELECT)
    {
        return false;
    }
    preset_base direction = (event == USER_PREVIOUS) ? -1 : 1;
    unsigned long now = millis();
    unsigned long diff = now - last;
    last = now;
    if (diff < TICK_THRESHOLD)
    {
        if (++tickCounter > TICK_BASE)
        {
            direction *= tickFactor();
        }
    }
    else
    {
        tickCounter = 0;
    }
    Set(Get() + direction);
    return true;
}

preset_base EndPoint::tickFactor()
{
    preset_base factor = tickCounter - TICK_BASE;
    return 2 * factor;
}

void EndPoint::Setup(uint8_t i, uint8_t c, uint8_t p)
{
    index = i;
    command = c;
    parameter = p;
}
