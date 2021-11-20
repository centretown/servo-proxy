// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedWriter.h"
#include "LedSegment.h"

#define ERR_STRIP_OK 0
#define ERR_STRIP_NOT_ENOUGH_ARGS 1
#define ERR_STRIP_NOT_FOUND 2
#define ERR_STRIP_INDEX 3
#define ERR_STRIP_BAD_VALUE 4

#define NO_BORDER 0xff

class LedStrip : public LedWriter
{
private:
    Adafruit_NeoPixel **strips = NULL;
    uint8_t nStrips = 0;
    LedSegment *segments = NULL;
    uint8_t nSegments = 0;
    uint16_t nPixels = 0;
    uint16_t borders[8] = {NO_BORDER};

public:
    LedStrip(Adafruit_NeoPixel **strips, uint8_t nStrips,
             LedSegment *segs, uint8_t nSegs);
    ~LedStrip();

    int process(const char *cmd);
    void setup();
    int loop();
    uint16_t numPixels() { return nPixels; }
    virtual void setPixelColor(uint16_t n, uint32_t c);
    virtual void show(uint16_t begin, uint16_t end);
};
