// Copyright (c) 2021 Dave Marsh. See LICENSE.
#if defined(USE_TOUCH_LIB)

#pragma once

#include "base.h"
#include "EventReader.h"
#include "TouchSensor.h"

class TouchReader : public EventReader
{
private:
    TouchSensor &touch;

public:
    TouchReader(TouchSensor &touch) : touch(touch) {}
    ~TouchReader() {}
    virtual UserEvent GetEvent()
    {
        touch.loop();
        switch (touch.GetState())
        {
        case TOUCH_TAP:
            return USER_NEXT;
        case TOUCH_HOLD:
            return USER_SELECT;
        default:
            break;
        }
        return USER_NOP;
    }
};

#endif
