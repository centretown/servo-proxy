// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

class Menu;

class EndPoint
{
private:
public:
    EndPoint() {}
    ~EndPoint() {}

    virtual void Setup(Menu *) = 0;
    virtual uint8_t Value(Menu *m) = 0;
    virtual void Exit(Menu *) = 0;
};
