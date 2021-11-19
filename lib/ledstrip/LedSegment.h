// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedWriter.h"

enum LedCommand
{
    STRIP_NOP,             // 0-do nothing quickly
    STRIP_RESET,           // 1-reset to blank
    STRIP_SOLID,           // 2
    STRIP_BLINK,           // 3
    STRIP_WIPE,            // 4
    STRIP_RAINBOW,         // 5
    STRIP_RAINBOW_CYCLE,   // 6
    STRIP_THEATRE,         // 7
    STRIP_THEATRE_RAINBOW, // 8
    STRIP_BRIGHTNESS,      // 9 default min 0, max 255, def 50
    STRIP_LAST
};

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

class LedSegment;
typedef void (LedSegment::*LedFunc)() const;

class LedSegment
{
private:
    uint16_t begin;
    uint16_t end;
    uint32_t wheel(byte WheelPos);
    LedWriter *ledWriter;
    // Adafruit_NeoPixel *strip;
    uint16_t count = 0;
    uint16_t theatreInc = 3;
    led_cmd_t cmd;
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
    void rainbow();
    void rainbowCycle();
    void theaterChase();
    void theaterChaseRainbow();
};
