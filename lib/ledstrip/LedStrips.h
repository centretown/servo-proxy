// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedWriter.h"
#include "LedSegment.h"

#if defined(SHARED_ERROR)
#define ERR_STRIP_OK ERR_OK
#define ERR_STRIP_NOT_ENOUGH_ARGS ERR_NOT_ENOUGH_ARGS
#define ERR_STRIP_NOT_FOUND ERR_NOT_FOUND
#define ERR_STRIP_INDEX ERR_INDEX
#define ERR_STRIP_BAD_VALUE ERR_BAD_VALUE
#else
#define ERR_STRIP_OK 0
#define ERR_STRIP_NOT_ENOUGH_ARGS 1
#define ERR_STRIP_NOT_FOUND 2
#define ERR_STRIP_INDEX 3
#define ERR_STRIP_BAD_VALUE 4
#endif

#define NO_BORDER 0

#define MAX_STRIPS 8

typedef struct
{
    size_t begin = 0;
    size_t end = 0;
    Adafruit_NeoPixel *strip = NULL;
} ledStrip;

class LedStrips : public LedWriter
{
private:
    size_t nStrips = 0;
    ledStrip strips[MAX_STRIPS];
    LedSegment *segments = NULL;
    size_t nSegments = 0;
    size_t nPixels = 0;

public:
    LedStrips(Adafruit_NeoPixel **strips, size_t nStrips,
              LedSegment *segs, size_t nSegs);
    ~LedStrips();

    int process(const char *cmd);
    void setup();
    int loop();
    uint16_t numPixels() { return nPixels; }
    virtual void setPixelColor(uint16_t n, uint32_t c);
    virtual void show(uint16_t begin, uint16_t end);
};
#endif