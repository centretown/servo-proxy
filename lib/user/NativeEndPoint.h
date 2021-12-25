// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#pragma once

#include "base.h"
#include "EndPoint.h"
#include "NativeStrips.h"

class NativeEndPoint : public EndPoint
{
private:
    NativeStrips &strips;
    LedSegment *seg = NULL;

public:
    NativeEndPoint() {}
    ~NativeEndPoint() {}

    virtual void start();
    virtual int16_t GetCounter() { return counter; }
    virtual void SetCounter(int16_t c) { counter = c; }
};

#endif