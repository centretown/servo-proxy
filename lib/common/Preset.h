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
    virtual void Set(preset_base v) = 0;
    virtual preset_base Get() = 0;
    virtual preset_base High() = 0;
    virtual preset_base Filtered()
    {
        return Get();
    }
    virtual char *Format(char *buffer, size_t bufLen)
    {
        return PresetBaseFormat(buffer, bufLen, Get());
    }
};
