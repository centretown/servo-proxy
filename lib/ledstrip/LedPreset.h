// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "base.h"

enum LedParameter
{
    STRIP_PRESET_RED,
    STRIP_PRESET_GREEN,
    STRIP_PRESET_BLUE,
    STRIP_PRESET_REPEAT,
    STRIP_PRESET_DELAY,
    STRIP_PRESET_COUNT,
    STRIP_PRESET_FIRST = STRIP_PRESET_RED
};

class LedPreset
{
private:
    uint16_t preset[STRIP_PRESET_COUNT];

public:
    LedPreset() {}
    ~LedPreset() {}
    void Set(uint8_t red, uint8_t green, uint8_t blue,
             uint8_t repeat, uint16_t delay)
    {
        preset[STRIP_PRESET_RED] = red;
        preset[STRIP_PRESET_GREEN] = green;
        preset[STRIP_PRESET_BLUE] = blue;
        preset[STRIP_PRESET_REPEAT] = repeat;
        preset[STRIP_PRESET_DELAY] = delay;
    }

    void Set(uint8_t i, uint8_t value)
    {
        if (i < STRIP_PRESET_COUNT)
            preset[i] = value;
    }

    uint16_t Get(uint8_t i)
    {
        if (i < STRIP_PRESET_COUNT)
            return preset[i];
        return 0;
    }

    uint16_t Red()
    {
        return preset[STRIP_PRESET_RED];
    }
    uint16_t Green()
    {
        return preset[STRIP_PRESET_GREEN];
    }
    uint16_t Blue()
    {
        return preset[STRIP_PRESET_BLUE];
    }
    uint16_t Repeat()
    {
        return preset[STRIP_PRESET_REPEAT];
    }
    uint16_t Delay()
    {
        return preset[STRIP_PRESET_DELAY];
    }

    void SetRed(uint16_t d) { preset[STRIP_PRESET_RED] = d; }
    void SetGreen(uint16_t d) { preset[STRIP_PRESET_GREEN] = d; }
    void SetBlue(uint16_t d) { preset[STRIP_PRESET_BLUE] = d; }
    void SetRepeat(uint16_t d) { preset[STRIP_PRESET_REPEAT] = d; }
    void SetDelay(uint16_t d) { preset[STRIP_PRESET_DELAY] = d; }
};
