// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedSegment.h"

LedSegment::Entry LedSegment::entry_table[] = {
    {
        "Solid",
        SolidSetup,
        Solid,
        STRIP_PRESET_FLAG_COLOR | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Blink",
        BlinkSetup,
        Blink,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Wipe",
        ColorWipeSetup,
        ColorWipe,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Cycle",
        CycleSetup,
        Cycle,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Rainbow",
        RainbowSetup,
        Rainbow,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Chase",
        ChaseSetup,
        Chase,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Cyclechase",
        ChaseCycleSetup,
        ChaseCycle,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Xmas",
        ChaseXmasSetup,
        ChaseXmas,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_DELAY | STRIP_PRESET_FLAG_REPEAT,
    },
    {
        "Reset",
        nop,
        Reset,
        0,
    },
};

void LedSegment::Start(LedOperator op)
{
    if (!isOperator(op))
    {
        current = STRIP_NOP;
        return;
    }
    current = op;
    zeroCounters();
    entry_table[current].setup(this);
}

void LedSegment::Start(LedOperator op, const unsigned *parms, size_t nparms)
{
    if (!isOperator(op))
    {
        current = STRIP_NOP;
        return;
    }

    int colorCount = (nparms < 3) ? nparms : 3;
    for (int i = 0; i < colorCount; i++)
    {
        presets.Set(STRIP_PRESET_RED + i, parms[i]);
    }

    if (nparms > 3)
    {
        presets.SetDelay(parms[3]);
    }

    if (nparms > 4)
    {
        presets.SetRepeat(parms[4]);
    }

    Start(op);
}

void LedSegment::Tick()
{
    if (current == STRIP_NOP)
    {
        return;
    }

    uint64_t now = millis();
    if (next > now)
    {
        return;
    }

    entry_table[current].tick(this);

    if (!nextStep())
    {
        current = STRIP_RESET;
    }

    next = now + presets.Delay();
}

bool LedSegment::nextStep()
{
    for (uint16_t i = 0;
         i < sizeof(counters) / sizeof(counters[0]); i++)
    {
        led_index *p = counters + i;
        if (p->max == 0)
        {
            return false;
        }

        p->count++;
        if (p->count < p->max)
        {
            return true;
        }
        p->count = 0;
    }

    return false;
}

void LedSegment::solidSetup()
{
    auto repeat = presets.Repeat();
    counters[0].max = repeat;
    counters[1].max = repeat;
    counters[2].max = repeat;
    counters[3].max = repeat;
}

void LedSegment::solid()
{
    auto color = ledWriter->Color(presets.Red(), presets.Green(), presets.Blue());
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::blinkSetup()
{
    counters[0].max = 2;
    counters[1].max = presets.Repeat();
}

void LedSegment::blink()
{
    uint32_t color = 0;
    if (counters[0].count > 0)
    {
        color = ledWriter->Color(presets.Red(),
                                 presets.Green(),
                                 presets.Blue());
    }

    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::wipeSetup()
{
    counters[0].max = count();
    counters[1].max = 2;
    counters[2].max = presets.Repeat();
}

void LedSegment::wipe()
{
    uint16_t i = begin + counters[0].count;
    uint32_t color = 0;
    if (counters[1].count > 0)
    {
        color = ledWriter->Color(presets.Red(),
                                 presets.Green(),
                                 presets.Blue());
    }
    ledWriter->setPixelColor(i, color);
    ledWriter->show(i, i);
}

void LedSegment::cycleSetup()
{
    counters[0].max = count();
    counters[1].max = 256;
    counters[2].max = presets.Repeat();
}

void LedSegment::cycle()
{
    uint16_t i = begin + counters[0].count;
    uint16_t j = counters[1].count;
    ledWriter->setPixelColor(i, wheel((i + j) & 255));
    ledWriter->show(i, i);
}
void LedSegment::rainbowSetup()
{
    counters[0].max = 5;
    counters[1].max = presets.Repeat();
}

void LedSegment::rainbow()
{
    uint32_t color = ledWriter->Color(presets.Red(),
                                      presets.Green(),
                                      presets.Blue());
    uint16_t pixCount = count();
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, wheel(((i * 256 / pixCount) + color) & 255));
    }
    ledWriter->show(begin, end);
}

void LedSegment::chaseSetup()
{
    counters[0].max = 2;
    counters[1].max = 3;
    counters[2].max = presets.Repeat();
}

void LedSegment::chase()
{
    uint32_t color = 0;
    if (counters[0].count == 0)
    {
        color = ledWriter->Color(presets.Red(),
                                 presets.Green(),
                                 presets.Blue());
    }
    uint16_t step = counters[1].count;
    for (uint16_t i = begin + step; i <= end; i += theatreInc)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::chaseXmasSetup()
{
    counters[0].max = 2;
    counters[1].max = 3;
    counters[2].max = 256;
    counters[3].max = presets.Repeat();
}

void LedSegment::chaseXmas()
{
    uint32_t color =
        (counters[0].count == 0)
            ? ledWriter->Color(presets.Red(), 0, presets.Blue())
            : ledWriter->Color(0, presets.Green(), presets.Blue());
    uint16_t step = counters[1].count;
    for (uint16_t i = begin + step; i <= end; i += theatreInc)
    {
        ledWriter->setPixelColor(i, color);
    }
    ledWriter->show(begin, end);
}

void LedSegment::chaseCycleSetup()
{
    counters[0].max = 2;
    counters[1].max = 3;
    counters[2].max = presets.Repeat();
}

void LedSegment::chaseCycle()
{
    uint16_t phase = counters[0].count;
    uint16_t step = counters[1].count;
    uint16_t index = counters[2].count;
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

void LedSegment::Reset()
{
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, 0);
    }
    ledWriter->show(begin, end);
    current = STRIP_NOP;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LedSegment::wheel(uint8_t pos)
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
