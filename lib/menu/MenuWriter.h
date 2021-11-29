// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <stdio.h>

class MenuWriter
{
private:
public:
    MenuWriter(){}
    ~MenuWriter(){}

    virtual uint16_t setSelection(uint16_t) = 0;
};
