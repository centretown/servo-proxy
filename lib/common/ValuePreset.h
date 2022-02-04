// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "Preset.h"

template <class T>
class ValuePreset : public Preset
{
protected:
    const char *label;
    T value;
    T high;

public:
    ValuePreset(const char *label,
                T value,
                T high) : label(label), value(value), high(high) {}

    virtual const char *Label() { return label; }
    virtual void Set(preset_base v)
    {
        if (v <= (preset_base)high)
            value = (T)v;
        else
            value = high;
    }
    virtual preset_base Get() { return (preset_base)value; }
    virtual preset_base High() { return (preset_base)high; }
};
