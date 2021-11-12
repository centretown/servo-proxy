// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedSegment.h"

LedSegment::LedSegment(uint16_t b, uint16_t e)
    : begin(b), end(e)
{
    if (begin > end)
    {
        begin = e;
        end = b;
    }
    count = end - begin + 1;
}

LedSegment::~LedSegment()
{
}

void LedSegment::setup(Adafruit_NeoPixel *s)
{
    strip = s;
}

void LedSegment::start(uint8_t command, const unsigned *parms, size_t nparms)
{
    int colorCount = nparms;
    unsigned colors[3] = {0};

    for (int i = 0; i < 3; i++)
    {
        if (i < colorCount)
        {
            colors[i] = parms[i] & 255;
        }
        else
        {
            colors[i] = 0;
        }
    }
    cmd.color = strip->Color(colors[0], colors[1], colors[2]);
    cmd.command = command;
    switch (command)
    {
    case STRIP_RESET:
        break;
    case STRIP_WIPE:
        cmd.index = 0;
        cmd.wait = 500;
        cmd.next = millis();
        break;
    case STRIP_RAINBOW:
        cmd.index = 0;
        cmd.color = 0;
        cmd.wait = 18;
        cmd.next = millis();
        break;
    case STRIP_RAINBOW_CYCLE:
        cmd.index = 0;
        cmd.color = 0;
        cmd.wait = 18;
        cmd.next = millis();
        break;
    case STRIP_THEATRE:
        cmd.index = 0;
        cmd.step = 0;
        cmd.phase = 0;
        cmd.wait = 50;
        cmd.next = millis();
        break;
    case STRIP_THEATRE_RAINBOW:
        cmd.index = 0;
        cmd.step = 0;
        cmd.phase = 0;
        cmd.wait = 25;
        cmd.next = millis();
        break;
    default:
        cmd.command = 0;
        break;
    }
}

void LedSegment::colorWipe()
{
    if (cmd.index >= count)
    {
        cmd.command = STRIP_RESET;
        return;
    }

    uint64_t now = millis();
    if (cmd.next > now)
    {
        return;
    }

    strip->setPixelColor(cmd.index, cmd.color);
    strip->show();
    cmd.index++;
    cmd.next = now + cmd.wait;
}

void LedSegment::rainbow()
{
    if (cmd.index >= count)
    {
        cmd.index = 0;
        cmd.color++;
    }

    if (cmd.color >= 256)
    {
        cmd.command = STRIP_RESET;
        return;
    }

    uint64_t now = millis();
    if (cmd.next > now)
    {
        return;
    }

    uint16_t i = cmd.index;
    uint16_t j = cmd.color;

    strip->setPixelColor(i, wheel((i + j) & 255));
    strip->show();
    cmd.index++;
    cmd.next = now + cmd.wait;
}

void LedSegment::rainbowCycle()
{
    if (cmd.color >= 256 * 5)
    {
        cmd.command = STRIP_RESET;
        return;
    }

    uint64_t now = millis();
    if (cmd.next > now)
    {
        return;
    }

    for (uint16_t i = begin; i <= end; i++)
    {
        strip->setPixelColor(i, wheel(((i * 256 / count) + cmd.color) & 255));
    }

    strip->show();
    cmd.color++;
    cmd.next = now + cmd.wait;
}

void LedSegment::theaterChase()
{
    if (cmd.step > 2)
    {
        cmd.step = 0;
        cmd.index++;
    }

    if (cmd.index > 255)
    {
        cmd.command = STRIP_RESET;
        return;
    }

    uint64_t now = millis();
    if (cmd.next > now)
    {
        return;
    }

    if (cmd.phase == 0)
    {
        cmd.phase = 1;
        for (uint16_t i = begin; i <= end; i += theatreInc)
        {
            strip->setPixelColor(i + cmd.step, cmd.color);
        }
    }
    else if (cmd.phase == 1)
    {
        cmd.phase = 0;
        for (uint16_t i = begin; i <= end; i += theatreInc)
        {
            strip->setPixelColor(i + cmd.step, 0);
        }
    }

    strip->show();
    cmd.step++;
    cmd.next = now + cmd.wait;
}

void LedSegment::theaterChaseRainbow()
{
    if (cmd.step > 2)
    {
        cmd.step = 0;
        cmd.index++;
    }

    if (cmd.index > 255)
    {
        cmd.command = STRIP_RESET;
        return;
    }

    uint64_t now = millis();
    if (cmd.next > now)
    {
        return;
    }

    if (cmd.phase == 0)
    {
        cmd.phase = 1;
        for (uint16_t i = begin; i <= end; i += theatreInc)
        {
            strip->setPixelColor(i + cmd.step,
                                 wheel((i + cmd.index) % 255));
        }
    }
    else if (cmd.phase == 1)
    {
        cmd.phase = 0;
        for (uint16_t i = begin; i <= end; i += theatreInc)
        {
            strip->setPixelColor(i + cmd.step, 0);
        }
    }

    strip->show();
    cmd.step++;
    cmd.next = now + cmd.wait;
}

void LedSegment::reset()
{
    for (uint16_t i = begin; i <= end; i++)
    {
        strip->setPixelColor(i, 0);
    }
    strip->show();
    cmd.command = 0;
    cmd.color = 0;
}

void LedSegment::loop()
{
    switch (cmd.command)
    {
    case STRIP_RESET:
        reset();
        break;

    case STRIP_WIPE:
        colorWipe();
        break;

    case STRIP_RAINBOW:
        rainbow();
        break;

    case STRIP_RAINBOW_CYCLE:
        rainbowCycle();
        break;

    case STRIP_THEATRE:
        theaterChase();
        break;

    case STRIP_THEATRE_RAINBOW:
        theaterChaseRainbow();
        break;

    default:
        break;
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LedSegment::wheel(byte pos)
{
    pos = 255 - pos;
    if (pos < 85)
    {
        return strip->Color(255 - pos * 3, 0, pos * 3);
    }
    if (pos < 170)
    {
        pos -= 85;
        return strip->Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return strip->Color(pos * 3, 255 - pos * 3, 0);
}
