// Copyright (c) 2021 Dave Marsh. See LICENSE.
#if defined(USE_TOUCH_LIB)

#pragma once

#include "base.h"
#include "MenuReader.h"
#include "TouchSensor.h"

class TouchMenuReader : public MenuReader
{
private:
    TouchSensor &touch;

public:
    TouchMenuReader(TouchSensor &touch) : touch(touch) {}
    ~TouchMenuReader() {}
    virtual MenuEvent GetEvent()
    {
        touch.loop();
        switch (touch.GetState())
        {
        case TOUCH_TAP:
            return MENU_NEXT;
        case TOUCH_HOLD:
            return MENU_PREVIOUS;
        }
        return MENU_NOP;
    }
};

#endif
