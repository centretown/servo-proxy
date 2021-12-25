// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#pragma once

#include "base.h"
#include "LedWriter.h"
#include "LedSegment.h"

class NativeStrips : public LedWriter
{
private:
    LedSegment segments[4];
    size_t nSegments = 0;

public:
    NativeStrips(/* args */);
    ~NativeStrips();
    virtual void setPixelColor(uint16_t n, uint32_t c);
    virtual void show(uint16_t begin, uint16_t end);
};

#endif