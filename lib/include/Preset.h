// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

class Preset
{
private:
public:
    Preset() {}
    virtual ~Preset() {}
    virtual const char *Label() = 0;
    virtual void Set(int16_t v) = 0;
    virtual int16_t Get() = 0;
    virtual int16_t Low() = 0;
    virtual int16_t High() = 0;
};
