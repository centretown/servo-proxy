// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedSegment.h"

LedOperation LedSegment::operations[] = {
    {
        "Solid",
        solidSetup,
        solid,
        STRIP_PRESET_FLAG_COLOR | STRIP_PRESET_FLAG_TIMER,
    },
    {
        "Blink",
        blinkSetup,
        blink,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_PULSE | STRIP_PRESET_FLAG_TIMER,
    },
    {
        "Wipe",
        colorWipeSetup,
        colorWipe,
        STRIP_PRESET_FLAG_COLOR | STRIP_PRESET_FLAG_BG |
            STRIP_PRESET_FLAG_PULSE | STRIP_PRESET_FLAG_TIMER,
    },
    {
        "Cycle",
        cycleSetup,
        cycle,
        STRIP_PRESET_FLAG_COLOR | STRIP_PRESET_FLAG_BG |
            STRIP_PRESET_FLAG_PULSE_CYCLE | STRIP_PRESET_FLAG_TIMER_CYCLE,
    },
    {
        "Rainbow",
        rainbowSetup,
        rainbow,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_PULSE | STRIP_PRESET_FLAG_TIMER,
    },
    {
        "Chase",
        chaseSetup,
        chase,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_PULSE | STRIP_PRESET_FLAG_TIMER,
    },
    {
        "Cyclechase",
        chaseCycleSetup,
        chaseCycle,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_PULSE_CYCLE | STRIP_PRESET_FLAG_TIMER_CYCLE,
    },
    {
        "Xmas",
        chaseXmasSetup,
        chaseXmas,
        STRIP_PRESET_FLAG_COLOR |
            STRIP_PRESET_FLAG_PULSE | STRIP_PRESET_FLAG_TIMER,
    },
    {
        "Reset",
        nop,
        reset,
        0,
    },
};

uint8_t LedSegment::GetPresetCount(LedOperator op)
{
    uint8_t count = 0;
    LedOperation &operation = operations[op];
    for (uint8_t id = 0; id < STRIP_PRESET_COUNT; id++)
    {
        if (operation.HasPreset(id))
        {
            count++;
        }
    }
    return count;
}

Preset *LedSegment::GetPreset(LedOperator op, uint8_t index)
{
    LedOperation &operation = operations[op];
    uint8_t countDown = index + 1;
    uint16_t id = 0;
    while (1)
    {
        if (operation.HasPreset(id))
        {
            if (--countDown == 0)
            {
                return presets.GetPreset((LedPropertyID)id);
            }
        }
        id++;
    }
    return NULL;
}

void LedSegment::Start(LedOperator op)
{
    if (!isOperator(op))
    {
        current = STRIP_NOP;
        return;
    }
    current = op;
    zeroCounters();
    operations[current].setup(this);
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
        presets.Set(STRIP_PRESET_PULSE, parms[3]);
    }

    if (nparms > 4)
    {
        presets.Set(STRIP_PRESET_REPEAT, parms[4]);
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

    operations[current].tick(this);

    if (!nextStep())
    {
        current = STRIP_RESET;
    }

    next = now + presets.Pulse();
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
    counters[0].max = pixelCount;
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
    counters[0].max = pixelCount;
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
    for (uint16_t i = begin; i <= end; i++)
    {
        ledWriter->setPixelColor(i, wheel(((i * 256 / pixelCount) + color) & 255));
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
