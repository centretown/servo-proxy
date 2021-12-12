// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

#include "MenuEvent.h"

class MenuReader
{
public:
    MenuReader() {}
    ~MenuReader() {}
    virtual MenuEvent GetEvent() = 0;
};
