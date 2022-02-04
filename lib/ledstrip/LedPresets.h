// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "ColorPreset.h"
#include "CountPreset.h"
#include "DimmerPreset.h"
#include "TimerPreset.h"

enum LedPropertyID
{
    STRIP_PRESET_DIMMER,
    STRIP_PRESET_RED,
    STRIP_PRESET_GREEN,
    STRIP_PRESET_BLUE,
    STRIP_PRESET_PULSE,
    STRIP_PRESET_PULSE_CYCLE,
    STRIP_PRESET_BG_DIMMER,
    STRIP_PRESET_BG_RED,
    STRIP_PRESET_BG_GREEN,
    STRIP_PRESET_BG_BLUE,
    STRIP_PRESET_REPEAT,
    STRIP_PRESET_TIMER,
    STRIP_PRESET_TIMER_CYCLE,
    STRIP_PRESET_COUNT,
};

enum LedPropertyFlag
{
    STRIP_PRESET_FLAG_DIMMER = 0x0001,
    STRIP_PRESET_FLAG_RED = 0x0002,
    STRIP_PRESET_FLAG_GREEN = 0x0004,
    STRIP_PRESET_FLAG_BLUE = 0x0008,
    STRIP_PRESET_FLAG_PULSE = 0x010,
    STRIP_PRESET_FLAG_PULSE_CYCLE = 0x0020,
    STRIP_PRESET_FLAG_BG_DIMMER = 0x0040,
    STRIP_PRESET_FLAG_BG_RED = 0x0080,
    STRIP_PRESET_FLAG_BG_GREEN = 0x0100,
    STRIP_PRESET_FLAG_BG_BLUE = 0x0200,
    STRIP_PRESET_FLAG_REPEAT = 0x0400,
    STRIP_PRESET_FLAG_TIMER = 0x0800,
    STRIP_PRESET_FLAG_TIMER_CYCLE = 0x1000,
    STRIP_PRESET_FLAG_COLOR = STRIP_PRESET_FLAG_DIMMER | STRIP_PRESET_FLAG_RED |
                              STRIP_PRESET_FLAG_GREEN | STRIP_PRESET_FLAG_BLUE,
    STRIP_PRESET_FLAG_BG = STRIP_PRESET_FLAG_BG_DIMMER | STRIP_PRESET_FLAG_BG_RED |
                           STRIP_PRESET_FLAG_BG_GREEN | STRIP_PRESET_FLAG_BG_BLUE,
};

enum LedText
{
    LED_DIMMER,
    LED_RED,
    LED_GREEN,
    LED_BLUE,
    LED_PULSE,
    LED_REPEAT,
    LED_TIMER,
};

class LedPresets
{
private:
    Preset *presets[STRIP_PRESET_COUNT];

private:
    static const char *Labels[];

public:
    inline static const char *Label(LedText txt)
    {
        return Labels[txt];
    }

    inline Preset *GetPreset(LedPropertyID id)
    {
        return presets[id];
    }

public:
    LedPresets();
    ~LedPresets();
    void Set(uint8_t red, uint8_t green, uint8_t blue,
             uint8_t repeat, uint16_t delay);

    void Set(uint8_t i, uint16_t value);

    uint16_t Get(uint8_t i)
    {
        if (i < STRIP_PRESET_COUNT)
            return presets[i]->Get();
        return 0;
    }

    uint8_t Dimmer()
    {
        return presets[STRIP_PRESET_DIMMER]->Filtered();
    }
    uint8_t Red()
    {
        return presets[STRIP_PRESET_RED]->Filtered();
    }
    uint8_t Green()
    {
        return presets[STRIP_PRESET_GREEN]->Filtered();
    }
    uint8_t Blue()
    {
        return presets[STRIP_PRESET_BLUE]->Filtered();
    }
    uint8_t Pulse()
    {
        return presets[STRIP_PRESET_PULSE]->Get();
    }
    uint8_t PulseCycle()
    {
        return presets[STRIP_PRESET_PULSE_CYCLE]->Get();
    }
    uint8_t Timer()
    {
        return presets[STRIP_PRESET_TIMER]->Get();
    }
    uint8_t TimerCycle()
    {
        return presets[STRIP_PRESET_TIMER_CYCLE]->Get();
    }
    uint8_t DimmerBG()
    {
        return presets[STRIP_PRESET_BG_DIMMER]->Filtered();
    }
    uint8_t RedBG()
    {
        return presets[STRIP_PRESET_BG_RED]->Filtered();
    }
    uint8_t GreenBG()
    {
        return presets[STRIP_PRESET_BG_GREEN]->Filtered();
    }
    uint8_t BlueBG()
    {
        return presets[STRIP_PRESET_BG_BLUE]->Filtered();
    }
    uint8_t Repeat()
    {
        return presets[STRIP_PRESET_REPEAT]->Get();
    }
};
