// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedPresets.h"

LedPresets::LedPresets()
{
    presets[STRIP_PRESET_RED] = new ColorPreset("Red", 63);
    presets[STRIP_PRESET_GREEN] = new ColorPreset("Green", 63);
    presets[STRIP_PRESET_BLUE] = new ColorPreset("Blue", 63);
    presets[STRIP_PRESET_RED1] = new ColorPreset("Red", 63);
    presets[STRIP_PRESET_GREEN1] = new ColorPreset("Green", 63);
    presets[STRIP_PRESET_BLUE1] = new ColorPreset("Blue", 63);
    presets[STRIP_PRESET_REPEAT] = new CountPreset("Repeat", 10);
    presets[STRIP_PRESET_DELAY] = new CountPreset("Delay", 10);
    presets[STRIP_PRESET_REPEAT1] = new CountPreset("Repeat", 10);
    presets[STRIP_PRESET_DELAY1] = new CountPreset("Delay", 10);
}

LedPresets::~LedPresets()
{
    delete presets[STRIP_PRESET_RED];
    delete presets[STRIP_PRESET_GREEN];
    delete presets[STRIP_PRESET_BLUE];
    delete presets[STRIP_PRESET_REPEAT];
    delete presets[STRIP_PRESET_DELAY];
}

void LedPresets::Set(uint8_t red, uint8_t green, uint8_t blue,
                     uint8_t repeat, uint16_t delay)
{
    presets[STRIP_PRESET_RED]->Set(red);
    presets[STRIP_PRESET_GREEN]->Set(green);
    presets[STRIP_PRESET_BLUE]->Set(blue);
    presets[STRIP_PRESET_REPEAT]->Set(repeat);
    presets[STRIP_PRESET_DELAY]->Set(delay);
}