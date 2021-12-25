// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO
#ifdef USE_LEDSTRIP_LIB

#pragma once

#include "LedStrips.h"
#include "EndPoint.h"

class LedEndPoint : public EndPoint
{
private:
    LedStrips &strips;
    LedSegment *seg = NULL;

public:
    LedEndPoint(LedStrips &strips) : strips(strips) {}
    ~LedEndPoint() {}

    virtual void setup(uint8_t i, uint8_t c, uint8_t p)
    {
        EndPoint::setup(i, c, p);
        seg = strips.Segment(index);
    }

    virtual void start()
    {
        seg->start(command);
    }

    virtual int16_t GetCounter()
    {
        if (parameter < STRIP_PRESET_COUNT)
        {
            LedPreset *preset = seg->getPreset(command);
            return preset->Get(parameter);
        }
        return 0;
    }

    virtual void SetCounter(int16_t c)
    {
        if (parameter < STRIP_PRESET_COUNT)
        {
            LedPreset *preset = seg->getPreset(command);
            preset->Set(parameter, c);
        }
    }
};

#endif // USE_LEDSTRIP_LIB
#endif // ARDUINO