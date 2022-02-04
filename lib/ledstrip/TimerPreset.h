// Copyright (c) 2022 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "ValuePreset.h"

#define TIMER_PRESET_HOUR 3600
#define TIMER_PRESET_MINUTE 60

class TimerPreset : public ValuePreset<uint32_t>
{
private:
    Preset *pulse = NULL;
    Preset *repeat = NULL;

public:
    TimerPreset(const char *label, Preset *pulse, Preset *repeat);
    virtual ~TimerPreset() {}
    virtual void Set(preset_base v);
    virtual char *Format(char *buffer, size_t bufLen);
    uint8_t Hours();
    uint8_t Minutes();
    uint8_t Seconds();
};
