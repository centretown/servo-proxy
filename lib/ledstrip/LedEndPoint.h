// Copyright (c) 2021 Dave Marsh. See LICENSE.
// #ifdef ARDUINO
#pragma once

#include "LedSegment.h"
#include "EndPoint.h"

class LedEndPoint : public EndPoint
{
private:
    // LedStrips &strips;
    LedSegment *segments = NULL;
    size_t segCount = 0;

    LedSegment *seg = NULL;

public:
    LedEndPoint(LedSegment *segments,
                size_t segCount) : segments(segments),
                                   segCount(segCount) {}
    ~LedEndPoint() {}

    virtual void setup(uint8_t index, uint8_t command, uint8_t parameter);
    virtual void start();
    virtual int16_t GetCounter();
    virtual void SetCounter(int16_t c);
};
// #endif