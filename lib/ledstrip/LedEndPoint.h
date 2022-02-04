// Copyright (c) 2021 Dave Marsh. See LICENSE.
#pragma once

#include "base.h"
#include "Preset.h"
#include "LedSegment.h"
#include "EndPoint.h"

class LedEndPoint : public EndPoint
{
private:
    LedSegment *segments = NULL;
    size_t segCount = 0;
    LedSegment *current = NULL;
    Preset *preset()
    {
        return current->GetPreset((LedOperator)command, parameter);
    }

public:
    LedEndPoint(LedSegment *segments, size_t segCount);
    ~LedEndPoint() {}

    virtual void Setup(uint8_t index, uint8_t command, uint8_t parameter);
    virtual void Start();
    virtual void SetIndex(uint8_t i);

    virtual preset_base Get();
    virtual preset_base High();
    virtual void Set(preset_base c);
};
