// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "ValuePreset.h"

class ColorPreset : public ValuePreset<uint8_t>
{
private:
    Preset *filter = NULL;

public:
    ColorPreset(const char *label,
                uint8_t value,
                uint8_t high = 0xff,
                Preset *f = NULL) : ValuePreset(label, value, high)
    {
        filter = f;
    }
    ~ColorPreset() {}

    preset_base Filtered();
};

