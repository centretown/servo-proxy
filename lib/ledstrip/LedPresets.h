// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "LedPreset.h"

enum LedPropertyID
{
    STRIP_PRESET_RED,
    STRIP_PRESET_GREEN,
    STRIP_PRESET_BLUE,
    STRIP_PRESET_REPEAT,
    STRIP_PRESET_DELAY,
    STRIP_PRESET_RED1,
    STRIP_PRESET_GREEN1,
    STRIP_PRESET_BLUE1,
    STRIP_PRESET_REPEAT1,
    STRIP_PRESET_DELAY1,
    STRIP_PRESET_COUNT,
    STRIP_PRESET_FIRST = STRIP_PRESET_RED,
};

enum LedPropertyFlag
{
    STRIP_PRESET_FLAG_RED = 0x0001,
    STRIP_PRESET_FLAG_GREEN = 0x0002,
    STRIP_PRESET_FLAG_BLUE = 0x0004,
    STRIP_PRESET_FLAG_REPEAT = 0x0008,
    STRIP_PRESET_FLAG_DELAY = 0x010,
    STRIP_PRESET_FLAG_RED1 = 0x0020,
    STRIP_PRESET_FLAG_GREEN1 = 0x0040,
    STRIP_PRESET_FLAG_BLUE1 = 0x0080,
    STRIP_PRESET_FLAG_REPEAT1 = 0x0100,
    STRIP_PRESET_FLAG_DELAY1 = 0x0200,
    STRIP_PRESET_FLAG_COLOR = STRIP_PRESET_FLAG_RED | STRIP_PRESET_FLAG_GREEN | STRIP_PRESET_FLAG_BLUE,
};

class LedPresets
{
private:
    // uint16_t preset[STRIP_PRESET_COUNT];
    Preset *presets[STRIP_PRESET_COUNT];

public:
    LedPresets();
    ~LedPresets();
    void Set(uint8_t red, uint8_t green, uint8_t blue,
             uint8_t repeat, uint16_t delay);

    void Set(uint8_t i, uint16_t value)
    {
        if (i < STRIP_PRESET_COUNT)
            presets[i]->Set(value);
    }

    uint16_t Get(uint8_t i)
    {
        if (i < STRIP_PRESET_COUNT)
            return presets[i]->Get();
        return 0;
    }

    uint16_t Red()
    {
        return presets[STRIP_PRESET_RED]->Get();
    }
    uint16_t Green()
    {
        return presets[STRIP_PRESET_GREEN]->Get();
    }
    uint16_t Blue()
    {
        return presets[STRIP_PRESET_BLUE]->Get();
    }
    uint16_t Repeat()
    {
        return presets[STRIP_PRESET_REPEAT]->Get();
    }
    uint16_t Delay()
    {
        return presets[STRIP_PRESET_DELAY]->Get();
    }

    void SetRed(uint16_t d) { presets[STRIP_PRESET_RED]->Set(d); }
    void SetGreen(uint16_t d) { presets[STRIP_PRESET_GREEN]->Set(d); }
    void SetBlue(uint16_t d) { presets[STRIP_PRESET_BLUE]->Set(d); }
    void SetRepeat(uint16_t d) { presets[STRIP_PRESET_REPEAT]->Set(d); }
    void SetDelay(uint16_t d) { presets[STRIP_PRESET_DELAY]->Set(d); }
};
