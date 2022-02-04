// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "ValuePreset.h"

class DimmerPreset : public ValuePreset<uint8_t>
{
public:
    DimmerPreset(const char *label,
                 uint8_t value = 100,
                 uint8_t high = 100) : ValuePreset(label, value, high) {}
    virtual ~DimmerPreset() {}
};
