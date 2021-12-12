// Copyright (c) 2021 Dave Marsh. See LICENSE.

#if defined(USE_ROTARY_LIB)

#pragma once

#include "base.h"
#include "MenuReader.h"
#include "Rotary.h"

class RotaryMenuReader : public MenuReader
{
private:
    Rotary &rotary;

public:
    RotaryMenuReader(Rotary &rotary) : rotary(rotary) {}
    ~RotaryMenuReader() {}
    virtual MenuEvent GetEvent()
    {
        rotary.loop();
        switch (rotary.GetState())
        {
        case ROTARY_COUNTER_CLOCKWISE:
            return MENU_PREVIOUS;

        case ROTARY_CLOCKWISE:
            return MENU_NEXT;

        case ROTARY_CLICK:
            return MENU_SELECT;
        }
        return MENU_NOP;
    }
};

#endif