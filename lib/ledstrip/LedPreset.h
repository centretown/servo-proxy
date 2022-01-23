// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "Preset.h"

template <class T>
class LedPreset : public Preset
{
private:
    const char *label;
    T value;
    T low;
    T high;

public:
    LedPreset(const char *label,
              T value,
              T low,
              T high) : label(label), value(value), low(low), high(high) {}

    const char *Label() { return label; }
    void Set(int16_t v) { value = (T)v; }
    int16_t Get() { return (int16_t)value; }
    int16_t Low() { return (int16_t)low; }
    int16_t High() { return (int16_t)high; }
};

class ColorPreset : public LedPreset<uint8_t>
{
public:
    ColorPreset(const char *label,
                uint8_t value,
                uint8_t low = 0,
                uint8_t high = 0xff) : LedPreset(label, value, low, high) {}
    ~ColorPreset() {}
};

class CountPreset : public LedPreset<uint16_t>
{
public:
    CountPreset(const char *label,
                uint16_t value,
                uint16_t low = 0,
                uint16_t high = 0xffff) : LedPreset(label, value, low, high) {}
    ~CountPreset() {}
};
