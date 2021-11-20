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

void LedSegment::setup(LedWriter *w)
{
    ledWriter = w;
}

void LedSegment::setupCmd()
{
    for (uint16_t i = 0; i < sizeof(cmd.indeces) / sizeof(cmd.indeces[0]); i++)
    {
        cmd.indeces[i].count = 0;
        cmd.indeces[i].max = 0;
    }
    switch (cmd.command)
    {
    case STRIP_RESET:
        break;

    case STRIP_SOLID:
        cmd.indeces[0].max = 200;
        break;

    case STRIP_BLINK:
        cmd.indeces[0].max = 1;
        cmd.indeces[1].max = 100;
        break;

    case STRIP_WIPE:
        cmd.indeces[0].max = end;
        break;

    case STRIP_RAINBOW:
        cmd.indeces[0].max = end;
        cmd.indeces[1].max = 256;
        break;

    case STRIP_RAINBOW_CYCLE:
        cmd.indeces[0].max = 256 * 5;
        break;

    case STRIP_THEATRE:
        cmd.indeces[0].max = 1;
        cmd.indeces[1].max = 2;
        cmd.indeces[2].max = 256;
        break;

    case STRIP_THEATRE_RAINBOW:
        cmd.indeces[0].max = 1;
        cmd.indeces[1].max = 2;
        cmd.indeces[2].max = 256;
        break;
    default:
        break;
    }
}

void LedSegment::start(uint8_t command, const unsigned *parms, size_t nparms)
{
    cmd.command = command;
    if (command == STRIP_NOP)
    {
        return;
    }

    unsigned colors[3] = {0};
    int colorCount = (nparms < 3) ? nparms : 3;

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
    cmd.color = ledWriter->Color(colors[0], colors[1], colors[2]);

    cmd.wait = 50;
    if (nparms > 3)
    {
        cmd.wait = parms[3];
    }

    cmd.repeat = 1;
    if (nparms > 4)
    {
        cmd.repeat = parms[4];
    }

    setupCmd();

    cmd.next = millis();
}

void LedSegment::loop()
{
    if (cmd.command == STRIP_NOP)
    {
        return;
    }
    uint64_t now = millis();
    if (cmd.next > now)
    {
        return;
    }

    switch (cmd.command)
    {
    case STRIP_RESET:
        reset();
        return;

    case STRIP_SOLID:
        solid();
        break;

    case STRIP_BLINK:
        blink();
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

    if (!nextStep())
    {
        cmd.command = STRIP_RESET;
    }
    cmd.next = now + cmd.wait;
}

void LedSegment::solid()
{
    if (cmd.indeces[0].count > 0)
    {
        return;
    }

    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, cmd.color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::blink()
{
    uint32_t color = 0;
    if (cmd.indeces[0].count == 0)
    {
        color = cmd.color;
    }
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

bool LedSegment::nextStep()
{
    led_index *pidx;
    for (uint16_t i = 0; i < sizeof(cmd.indeces) / sizeof(cmd.indeces[0]); i++)
    {
        pidx = &cmd.indeces[i];
        if (pidx->max == 0)
        {
            return false;
        }

        if (pidx->count <= pidx->max)
        {
            pidx->count++;
            return true;
        }
        pidx->count = 0;
    }

    return false;
}

void LedSegment::colorWipe()
{
    uint16_t index = cmd.indeces[0].count;
    ledWriter->setPixelColor(index, cmd.color);
    ledWriter->show(index, index);
}

void LedSegment::rainbow()
{
    uint16_t i = cmd.indeces[0].count;
    uint16_t j = cmd.indeces[1].count;
    ledWriter->setPixelColor(i, wheel((i + j) & 255));
    ledWriter->show(i, i);
}

void LedSegment::rainbowCycle()
{
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, wheel(((i * 256 / count) + cmd.color) & 255));
    }
    ledWriter->show(begin, end);
}

void LedSegment::theaterChase()
{
    uint32_t color =
        (cmd.indeces[0].count == 0) ? cmd.color : 0;
    uint16_t step = cmd.indeces[1].count;

    for (uint16_t i = begin; i <= end; i += theatreInc)
    {
        ledWriter->setPixelColor(i + step, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::theaterChaseRainbow()
{
    uint16_t phase = cmd.indeces[0].count;
    uint16_t step = cmd.indeces[1].count;
    uint16_t index = cmd.indeces[2].count;
    if (phase == 0)
    {
        uint16_t i = begin;
        for (; i <= end; i += theatreInc)
        {
            ledWriter->setPixelColor(i + step, wheel((i + index) % 255));
        }
    }
    else
    {
        uint16_t i = begin;
        for (; i <= end; i += theatreInc)
        {
            ledWriter->setPixelColor(i + step, 0);
        }
    }
    ledWriter->show(begin, end);
}

void LedSegment::reset()
{
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, 0);
    }
    ledWriter->show(begin, end);
    cmd.command = STRIP_NOP;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LedSegment::wheel(byte pos)
{
    pos = 255 - pos;
    if (pos < 85)
    {
        return ledWriter->Color(255 - pos * 3, 0, pos * 3);
    }
    if (pos < 170)
    {
        pos -= 85;
        return ledWriter->Color(0, pos * 3, 255 - pos * 3);
    }
    pos -= 170;
    return ledWriter->Color(pos * 3, 255 - pos * 3, 0);
}
