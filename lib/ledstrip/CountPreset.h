// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "ValuePreset.h"

class CountPreset : public ValuePreset<uint16_t>
{
public:
    CountPreset(const char *label,
                uint16_t value,
                uint16_t high = 0xffff) : ValuePreset(label, value, high) {}
    ~CountPreset() {}
};
