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
    STRIP_OUT_OF_BOUNDS, // to big
    STRIP_FIRST = STRIP_SOLID
};

typedef struct
{
    uint8_t red = 127;
    uint8_t green = 127;
    uint8_t blue = 127;
    uint16_t wait = 50;       // milli-sec
    uint16_t duration = 1000; // milli-sec, 0 forever
} led_settings;

typedef struct
{
    uint16_t count = 0;
    uint16_t max = 0;
} led_index;

typedef struct
{
    uint8_t command = 0;
    uint8_t repeat = 0;
    uint16_t wait = 500;
    uint32_t color = 0;
    uint64_t next = 0L;
    led_index indeces[4];
} led_cmd_t;
