// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include "base.h"
#endif

class Menu;

class EndPoint
{
private:
public:
    EndPoint() {}
    ~EndPoint() {}

    virtual void Setup(Menu *) = 0;
    virtual void Show(Menu *m) = 0;
    virtual void Exit(Menu *) = 0;
};
