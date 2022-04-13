// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "LedEndPoint.h"

LedEndPoint::LedEndPoint(LedSegment *segments,
                         size_t segCount) : segments(segments),
                                            segCount(segCount)
{
    current = &segments[0];
}

void LedEndPoint::SetIndex(uint8_t i)
{
    if (i >= segCount)
    {
        return;
    }
    state[ENDPOINT_INDEX] = i;
    current = &segments[ENDPOINT_INDEX];
}

preset_base LedEndPoint::Get()
{
    return preset()->Get();
}

preset_base LedEndPoint::High()
{
    return preset()->High();
}

void LedEndPoint::Set(preset_base c)
{
    preset()->Set(c);
}

void LedEndPoint::Start()
{
    current->Start((LedOperator)state[ENDPOINT_RUN]);
}
