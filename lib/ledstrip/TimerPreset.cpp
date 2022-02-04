// Copyright (c) 2022 Dave Marsh. See LICENSE.

#include "TimerPreset.h"

TimerPreset::TimerPreset(
    const char *label,
    Preset *p,
    Preset *r) : ValuePreset(label, TIMER_PRESET_MINUTE, TIMER_PRESET_HOUR * 8)
{
    pulse = p;
    repeat = r;
    Set(value);
}

uint8_t TimerPreset::Hours()
{
    return Get() / TIMER_PRESET_HOUR;
}

uint8_t TimerPreset::Minutes()
{
    return (Get() % TIMER_PRESET_HOUR) / TIMER_PRESET_MINUTE;
}

uint8_t TimerPreset::Seconds()
{
    return (Get() % TIMER_PRESET_MINUTE);
}

void TimerPreset::Set(preset_base v)
{
    ValuePreset::Set(v);
    preset_base r = 1000 * Get();
    r /= pulse->Get();
    repeat->Set(r);
}

char *TimerPreset::Format(char *buffer, size_t bufLen)
{
    snprintf(buffer, bufLen, "%u:%02u:%02u",
             Hours(), Minutes(), Seconds());
    return buffer;
}