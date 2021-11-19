// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedWriter.h"

uint32_t LedWriter::Color(uint8_t r, uint8_t g, uint8_t b)
{
    return Adafruit_NeoPixel::Color(r, g, b);
}

uint32_t LedWriter::ColorHSV(uint16_t hue, uint8_t sat, uint8_t val)
{
    return Adafruit_NeoPixel::ColorHSV(hue, sat, val);
}

uint32_t LedWriter::gamma32(uint32_t x)
{
    return Adafruit_NeoPixel::gamma32(x);
}

uint8_t LedWriter::sine8(uint8_t x)
{
    return Adafruit_NeoPixel::sine8(x);
}

uint8_t LedWriter::gamma8(uint8_t x)
{
    return Adafruit_NeoPixel::gamma8(x);
}
