// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "LedWriter.h"
#include "LedCommand.h"

class LedSegment
{
public:
    uint16_t begin;
    uint16_t end;

private:
    uint32_t wheel(byte WheelPos);
    LedWriter *ledWriter;
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
    // void start(uint8_t command, led_cmd_t *p);
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