// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "UserEvent.h"

enum TickState : uint8_t
{
    TICK_STATE_NORMAL,
    TICK_STATE_ACCELERATE,
};

#define TICK_THRESHOLD 300
#define TICK_BASE 3

enum EndPointState : uint8_t
{
    ENDPOINT_INDEX,
    ENDPOINT_RUN,
    ENDPOINT_EDIT,
    ENDPOINT_NAV,
    ENDPOINT_BACK,
};

class EndPoint
{
protected:
    uint8_t state[3] = {0};
    uint8_t tickCounter = 0;

private:
    unsigned long last = 0;

public:
    EndPoint()
    {
        last = millis();
    }
    virtual ~EndPoint() {}

    virtual void Select(uint8_t hook, uint8_t value);
    virtual void Start() = 0;
    virtual bool Edit(UserEvent event);

    virtual preset_base Get() = 0;
    virtual preset_base High() = 0;
    virtual void Set(preset_base) = 0;
    virtual char *Format(char *buffer, size_t bufLen)
    {
        return PresetBaseFormat(buffer, bufLen, Get());
    }

    preset_base tickFactor();
};
