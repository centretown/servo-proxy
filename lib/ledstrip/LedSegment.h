// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define STRIP_FIRST 1
#define STRIP_RESET 1
#define STRIP_WIPE 2
#define STRIP_RAINBOW 3
#define STRIP_RAINBOW_CYCLE 4
#define STRIP_THEATRE 5
#define STRIP_THEATRE_RAINBOW 6
#define STRIP_LAST 6

typedef struct
{
    uint8_t command = 0;
    uint8_t phase = 0;
    uint8_t step = 0;
    uint16_t wait = 500;
    uint16_t index = 0;
    uint32_t color = 0;
    uint64_t next = 0L;
} led_cmd_t;

class LedSegment
{
private:
    uint16_t begin;
    uint16_t end;
    uint32_t wheel(byte WheelPos);
    Adafruit_NeoPixel *strip;
    uint16_t count = 0;
    uint16_t theatreInc = 3;
    led_cmd_t cmd;

public:
    LedSegment(uint16_t begin, uint16_t end);
    ~LedSegment();

    void setup(Adafruit_NeoPixel *strip);
    void start(uint8_t command, const unsigned *parms, size_t nparms);
    void loop();

    void reset();
    void colorWipe();
    void rainbow();
    void rainbowCycle();
    void theaterChase();
    void theaterChaseRainbow();
    void theater(uint32_t color);

};
