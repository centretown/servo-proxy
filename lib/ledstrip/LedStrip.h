// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedSegment.h"

#define ERR_STRIP_OK 0
#define ERR_STRIP_NOT_ENOUGH_ARGS 1
#define ERR_STRIP_NOT_FOUND 2
#define ERR_STRIP_INDEX 3
#define ERR_STRIP_BAD_VALUE 4

class LedStrip
{
private:
    Adafruit_NeoPixel &strip;
    LedSegment *segments;
    uint8_t nSegments;

public:
    LedStrip(Adafruit_NeoPixel &strip,
             LedSegment *segs, uint8_t nSegs);
    ~LedStrip();

    int process(const char *cmd);
    void setup();
    int loop();
};
