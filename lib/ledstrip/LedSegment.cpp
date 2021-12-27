// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

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
    setPreset(STRIP_SOLID, 63, 63, 63, 255, 255);
    setPreset(STRIP_BLINK, 63, 63, 63, 255, 100);
    setPreset(STRIP_WIPE, 63, 63, 63, 255, 250);
    setPreset(STRIP_CYCLE, 63, 63, 63, 255, 10);
    setPreset(STRIP_RAINBOW, 63, 63, 63, 255, 255);
    setPreset(STRIP_CHASE, 63, 63, 63, 255, 100);
    setPreset(STRIP_CHASE_CYCLE, 63, 63, 63, 255, 100);
    setPreset(STRIP_CHASE_XMAS, 63, 63, 0, 255, 300);
}

void LedSegment::setPreset(uint8_t command, uint8_t parameter, uint8_t value)
{
    LedPreset *preset = getPreset(command);
    preset->Set(parameter, value);
}

void LedSegment::setPreset(uint8_t command,
                           uint8_t red, uint8_t green, uint8_t blue,
                           uint8_t repeat, uint16_t delay)
{
    LedPreset *preset = getPreset(command);
    preset->Set(red, green, blue, repeat, delay);
}

void LedSegment::loadPreset(uint8_t command)
{
    if (command >= STRIP_FIRST && command <= STRIP_LAST)
    {
        LedPreset *preset = getPreset(command);
        loadPreset(preset);
    }
}

void LedSegment::loadPreset(LedPreset *preset)
{
    cmd.color = ledWriter->Color(preset->Red(), preset->Green(), preset->Blue());
    cmd.repeat = preset->Repeat();
    cmd.delay = preset->Delay();
}

void LedSegment::setupCmd()
{
    memset(cmd.indeces, 0, sizeof(cmd.indeces));
    switch (cmd.command)
    {
    case STRIP_RESET:
        break;
    case STRIP_SOLID:
        cmd.indeces[0].max = cmd.repeat;
        cmd.indeces[1].max = cmd.repeat;
        cmd.indeces[2].max = cmd.repeat;
        cmd.indeces[3].max = cmd.repeat;
        break;
    case STRIP_BLINK:
        cmd.indeces[0].max = 2;
        cmd.indeces[1].max = cmd.repeat;
        break;
    case STRIP_WIPE:
        cmd.indeces[0].max = count;
        cmd.indeces[1].max = 2;
        cmd.indeces[2].max = cmd.repeat;
        break;
    case STRIP_CYCLE:
        cmd.indeces[0].max = count;
        cmd.indeces[1].max = 256;
        cmd.indeces[2].max = cmd.repeat;
        break;
    case STRIP_RAINBOW:
        cmd.indeces[0].max = 5;
        cmd.indeces[1].max = cmd.repeat;
        break;
    case STRIP_CHASE:
        cmd.indeces[0].max = 2;
        cmd.indeces[1].max = 3;
        cmd.indeces[2].max = cmd.repeat;
        break;
    case STRIP_CHASE_CYCLE:
        cmd.indeces[0].max = 2;
        cmd.indeces[1].max = 3;
        cmd.indeces[2].max = cmd.repeat;
        break;
    case STRIP_CHASE_XMAS:
        cmd.indeces[0].max = 2;
        cmd.indeces[1].max = 3;
        cmd.indeces[2].max = 256;
        cmd.indeces[3].max = cmd.repeat;
        break;
    default:
        break;
    }
}

void LedSegment::start(uint8_t command)
{
    cmd.command = command;
    if (command < STRIP_FIRST || command > STRIP_LAST)
    {
        return;
    }
    LedPreset *preset = getPreset(command);
    loadPreset(preset);
    setupCmd();
    cmd.next = millis();
}

void LedSegment::start(uint8_t command, const unsigned *parms, size_t nparms)
{
    cmd.command = command;
    if (command < STRIP_FIRST || command > STRIP_LAST)
    {
        return;
    }

    LedPreset *preset = getPreset(command);
    int colorCount = (nparms < 3) ? nparms : 3;
    for (int i = 0; i < colorCount; i++)
    {
        preset->Set(STRIP_PRESET_RED + i, parms[i]);
    }

    if (nparms > 3)
    {
        preset->SetDelay(parms[3]);
    }

    if (nparms > 4)
    {
        preset->SetRepeat(parms[4]);
    }

    start(command);
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

    case STRIP_CYCLE:
        cycle();
        break;

    case STRIP_RAINBOW:
        rainbow();
        break;

    case STRIP_CHASE:
        chase();
        break;

    case STRIP_CHASE_CYCLE:
        chaseCycle();
        break;
    case STRIP_CHASE_XMAS:
        chaseXmas();
        break;
    default:
        break;
    }

    if (!nextStep())
    {
        cmd.command = STRIP_RESET;
    }
    cmd.next = now + cmd.delay;
}

void LedSegment::solid()
{
    auto preset = getPreset(cmd.command);
    cmd.color = ledWriter->Color(preset->Red(), preset->Green(), preset->Blue());

    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, cmd.color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::blink()
{
    uint32_t color = 0;
    if (cmd.indeces[0].count > 0)
    {
        auto preset = getPreset(cmd.command);
        cmd.color = ledWriter->Color(preset->Red(),
                                     preset->Green(),
                                     preset->Blue());
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
    for (uint16_t i = 0;
         i < sizeof(cmd.indeces) / sizeof(cmd.indeces[0]); i++)
    {
        pidx = cmd.indeces + i;
        if (pidx->max == 0)
        {
            return false;
        }

        pidx->count++;
        if (pidx->count < pidx->max)
        {
            return true;
        }
        pidx->count = 0;
    }

    return false;
}

void LedSegment::colorWipe()
{

    uint16_t i = begin + cmd.indeces[0].count;
    uint32_t color = 0;
    if (cmd.indeces[1].count > 0)
    {
        auto preset = getPreset(cmd.command);
        cmd.color = ledWriter->Color(preset->Red(),
                                     preset->Green(),
                                     preset->Blue());
        color = cmd.color;
    }
    ledWriter->setPixelColor(i, color);
    ledWriter->show(i, i);
}

void LedSegment::cycle()
{
    uint16_t i = begin + cmd.indeces[0].count;
    uint16_t j = cmd.indeces[1].count;
    ledWriter->setPixelColor(i, wheel((i + j) & 255));
    ledWriter->show(i, i);
}

void LedSegment::rainbow()
{
    auto preset = getPreset(cmd.command);
    cmd.color = ledWriter->Color(preset->Red(),
                                 preset->Green(),
                                 preset->Blue());
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, wheel(((i * 256 / count) + cmd.color) & 255));
    }
    ledWriter->show(begin, end);
}

void LedSegment::chase()
{
    uint32_t color = 0;
    if (cmd.indeces[0].count == 0)
    {
        auto preset = getPreset(cmd.command);
        cmd.color = ledWriter->Color(preset->Red(),
                                     preset->Green(),
                                     preset->Blue());
        color = cmd.color;
    }
    uint16_t step = cmd.indeces[1].count;
    for (uint16_t i = begin + step; i <= end; i += theatreInc)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::chaseXmas()
{
    LedPreset *preset = getPreset(cmd.command);
    uint32_t color =
        (cmd.indeces[0].count == 0)
            ? ledWriter->Color(preset->Red(), 0, preset->Blue())
            : ledWriter->Color(0, preset->Green(), preset->Blue());
    uint16_t step = cmd.indeces[1].count;
    for (uint16_t i = begin + step; i <= end; i += theatreInc)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::chaseCycle()
{
    uint16_t phase = cmd.indeces[0].count;
    uint16_t step = cmd.indeces[1].count;
    uint16_t index = cmd.indeces[2].count;
    if (phase == 0)
    {
        for (uint16_t i = begin + step; i <= end; i += theatreInc)
        {
            ledWriter->setPixelColor(i, wheel((i + index) % 255));
        }
    }
    else
    {
        for (uint16_t i = begin + step; i <= end; i += theatreInc)
        {
            ledWriter->setPixelColor(i, 0);
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

#endif
