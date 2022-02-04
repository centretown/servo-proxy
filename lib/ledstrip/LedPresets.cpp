// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedPresets.h"

const char *LedPresets::Labels[] = {
    "Dimmer",
    "Red",
    "Green",
    "Blue",
    "Pulse",
    "Repeat",
    "Timer",
};

LedPresets::LedPresets()
{
    DimmerPreset *dimmer = new DimmerPreset(Labels[LED_DIMMER], 100, 100);
    presets[STRIP_PRESET_DIMMER] = dimmer;
    presets[STRIP_PRESET_RED] = new ColorPreset(Labels[LED_RED], 63, 255, dimmer);
    presets[STRIP_PRESET_GREEN] = new ColorPreset(Labels[LED_GREEN], 63, 255, dimmer);
    presets[STRIP_PRESET_BLUE] = new ColorPreset(Labels[LED_BLUE], 63, 255, dimmer);

    CountPreset *pulse = new CountPreset(Labels[LED_PULSE], 200);
    presets[STRIP_PRESET_PULSE] = pulse;
    CountPreset *cyclePulse = new CountPreset(Labels[LED_PULSE], 10);
    presets[STRIP_PRESET_PULSE_CYCLE] = cyclePulse;

    dimmer = new DimmerPreset(Labels[LED_DIMMER], 100, 100);
    presets[STRIP_PRESET_BG_DIMMER] = dimmer;
    presets[STRIP_PRESET_BG_RED] = new ColorPreset(Labels[LED_RED], 0, 255, dimmer);
    presets[STRIP_PRESET_BG_GREEN] = new ColorPreset(Labels[LED_GREEN], 0, 255, dimmer);
    presets[STRIP_PRESET_BG_BLUE] = new ColorPreset(Labels[LED_BLUE], 0, 255, dimmer);

    ValuePreset<uint32_t> *repeat = new ValuePreset<uint32_t>(Labels[LED_REPEAT], 1000, 0xffffffff);
    presets[STRIP_PRESET_REPEAT] = repeat;
    presets[STRIP_PRESET_TIMER] =
        new TimerPreset(Labels[LED_TIMER], pulse, repeat);
    presets[STRIP_PRESET_TIMER_CYCLE] =
        new TimerPreset(Labels[LED_TIMER], cyclePulse, repeat);
}

LedPresets::~LedPresets()
{
    delete presets[STRIP_PRESET_DIMMER];
    delete presets[STRIP_PRESET_RED];
    delete presets[STRIP_PRESET_GREEN];
    delete presets[STRIP_PRESET_BLUE];
    delete presets[STRIP_PRESET_PULSE];
    delete presets[STRIP_PRESET_PULSE_CYCLE];
    delete presets[STRIP_PRESET_BG_DIMMER];
    delete presets[STRIP_PRESET_BG_RED];
    delete presets[STRIP_PRESET_BG_GREEN];
    delete presets[STRIP_PRESET_BG_BLUE];
    delete presets[STRIP_PRESET_REPEAT];
    delete presets[STRIP_PRESET_TIMER];
    delete presets[STRIP_PRESET_TIMER_CYCLE];
}

void LedPresets::Set(uint8_t red, uint8_t green, uint8_t blue,
                     uint8_t repeat, uint16_t delay)
{
    presets[STRIP_PRESET_RED]->Set(red);
    presets[STRIP_PRESET_GREEN]->Set(green);
    presets[STRIP_PRESET_BLUE]->Set(blue);
    presets[STRIP_PRESET_PULSE]->Set(delay);
    presets[STRIP_PRESET_REPEAT]->Set(repeat);
}

void LedPresets::Set(uint8_t i, uint16_t value)
{
    if (i >= STRIP_PRESET_COUNT)
    {
        return;
    }
    presets[i]->Set(value);
}
