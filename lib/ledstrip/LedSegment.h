// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once

#include "base.h"
// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>
#include "LedWriter.h"
#include "LedCommand.h"
#include "LedPreset.h"

class LedSegment
{
private:
    uint16_t begin;
    uint16_t end;

    uint32_t wheel(byte WheelPos);
    LedWriter *ledWriter;
    uint16_t count = 0;
    uint8_t theatreInc = 3;
    led_cmd_t cmd;
    LedPreset presets[STRIP_LAST - STRIP_FIRST + 1];
    bool nextStep();
    void setupCmd();
    void zeroIndex();
    void setPreset(uint8_t command, uint8_t parameter, uint8_t value);
    void setPreset(uint8_t command, uint8_t red, uint8_t green, uint8_t blue, uint8_t repeat, uint16_t wait);
    void loadPreset(uint8_t command);
    void loadPreset(LedPreset *p);
    inline LedPreset *getPreset(uint8_t command)
    {
        return &presets[command - STRIP_FIRST];
    }

    friend class LedStrips;
    friend class LedEndPoint;

public:
    LedSegment(uint16_t begin, uint16_t end);
    ~LedSegment();

    void setup(LedWriter *w);
    void start(uint8_t command, const unsigned *parms, size_t nparms);
    void start(uint8_t command);
    void loop();

    void reset();
    void solid();
    void blink();
    void colorWipe();
    void cycle();
    void rainbow();
    void chase();
    void chaseCycle();
    void chaseXmas();
};

#endif