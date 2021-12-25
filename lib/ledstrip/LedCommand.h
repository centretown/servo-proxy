// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

enum LedCommand
{
    STRIP_NOP,           // do nothing quickly
    STRIP_SOLID,         // solid light
    STRIP_BLINK,         // blink
    STRIP_WIPE,          // light sequentially
    STRIP_CYCLE,         // cycle thru colors
    STRIP_RAINBOW,       // visible spectrum across
    STRIP_CHASE,         // every 3rd light on and off
    STRIP_CHASE_CYCLE,   // chase combined with cycle
    STRIP_CHASE_XMAS,    // chase combined with xmas
    STRIP_RESET,         // reset to blank
    STRIP_OUT_OF_BOUNDS, // too big
    STRIP_FIRST = STRIP_SOLID,
    STRIP_LAST = STRIP_CHASE_XMAS
};

#define MAX_WAIT 0xffff
// enum LedPreset
// {
//     STRIP_PRESET_RED,
//     STRIP_PRESET_GREEN,
//     STRIP_PRESET_BLUE,
//     STRIP_PRESET_REPEAT,
//     STRIP_PRESET_DELAY,
//     STRIP_PRESET_COUNT,
//     STRIP_PRESET_FIRST = STRIP_PRESET_RED
// };

// typedef uint16_t led_preset[STRIP_PRESET_COUNT];

// typedef struct
// {
//     uint8_t red = 127;
//     uint8_t green = 127;
//     uint8_t blue = 127;
//     uint8_t repeat = 255; // milli-sec, 0 forever
//     uint16_t wait = 50;   // milli-sec
// } led_preset;

typedef struct
{
    uint16_t count = 0;
    uint16_t max = 0;
} led_index;

typedef struct
{
    uint8_t command = STRIP_NOP;
    uint32_t color = 0;
    uint8_t repeat = 0;
    uint16_t delay = 500;
    uint64_t next = 0L;
    led_index indeces[4];
} led_cmd_t;
