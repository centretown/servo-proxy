// Copyright (c) 2021 Dave Marsh. See LICENSE.

#if defined(USE_ROTARY_LIB)

#pragma once

#include "base.h"
#include "EventReader.h"
#include "Rotary.h"

class RotaryReader : public EventReader
{
private:
    Rotary &rotary;

public:
    RotaryReader(Rotary &rotary) : rotary(rotary) {}
    ~RotaryReader() {}
    virtual UserEvent GetEvent()
    {
        rotary.tick();
        switch (rotary.GetState())
        {
        case ROTARY_COUNTER_CLOCKWISE:
            return USER_PREVIOUS;

        case ROTARY_CLOCKWISE:
            return USER_NEXT;

        case ROTARY_CLICK:
            return USER_SELECT;

        default:
            break;
        }
        return USER_WAIT;
    }
};

#endif