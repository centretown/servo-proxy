// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedWriter.h"

enum LedCommand
{
    STRIP_NOP,         // 0-do nothing quickly
    STRIP_SOLID,       // 2-solid light
    STRIP_BLINK,       // 3-blink
    STRIP_WIPE,        // 4-light sequentially
    STRIP_CYCLE,       // 5-cycle thru colors
    STRIP_RAINBOW,     // 6-visible spectrum across
    STRIP_CHASE,       // 7-every 3rd light on and off
    STRIP_CHASE_CYCLE, // 8-chase combined with cycle
    STRIP_RESET,       // 1-reset to blank
    STRIP_OUT_OF_BOUNDS,
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

class LedSegment
{
public:
    uint16_t begin;
    uint16_t end;

private:
    uint32_t wheel(byte WheelPos);
    LedWriter *ledWriter;
    // Adafruit_NeoPixel *strip;
    uint16_t count = 0;
    uint8_t theatreInc = 3;
    led_cmd_t cmd;
    led_settings settings;
    bool nextStep();
    void setupCmd();

public:
    LedSegment(uint16_t begin, uint16_t end);
    ~LedSegment();

    void setup(LedWriter *w);
    void start(uint8_t command, const unsigned *parms, size_t nparms);
    void loop();

    void reset();
    void solid();
    void blink();
    void colorWipe();
    void cycle();
    void rainbow();
    void chase();
    void chaseCycle();
};

#endif