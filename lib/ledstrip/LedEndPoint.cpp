// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedEndPoint.h"

void LedEndPoint::setup(uint8_t i, uint8_t c, uint8_t p)
{
    EndPoint::setup(i, c, p);
    if (index < segCount)
    {
        seg = &segments[index];
    }
}

int16_t LedEndPoint::GetCounter()
{
    if (parameter < STRIP_PRESET_COUNT)
    {
        return seg->Presets().Get(parameter);
    }
    return 0;
}

void LedEndPoint::SetCounter(int16_t c)
{
    if (parameter < STRIP_PRESET_COUNT)
    {
        seg->Presets().Set(parameter, c);
    }
}

void LedEndPoint::start()
{
    seg->start((LedOperator)command);
}
