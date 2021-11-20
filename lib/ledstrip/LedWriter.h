// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LedWriter
{
public:
    virtual void setPixelColor(uint16_t n, uint32_t c) = 0;
    virtual void show(uint16_t begin, uint16_t end) = 0;

    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    static uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255);
    static uint32_t gamma32(uint32_t x);
    static uint8_t sine8(uint8_t x);
    static uint8_t gamma8(uint8_t x);
};
