// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "EndPoint.h"

bool EndPoint::Edit(UserEvent event)
{
    if (event == USER_SELECT)
    {
        return true;
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
    return false;
}

preset_base EndPoint::tickFactor()
{
    preset_base factor = tickCounter - TICK_BASE;
    return 2 * factor;
}

void EndPoint::Select(uint8_t index, uint8_t value)
{
    if (index < ENDPOINT_NAV)
    {
        state[index] = value;
    }
}