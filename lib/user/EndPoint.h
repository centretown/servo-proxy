// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "UserEvent.h"

enum TickState
{
    TICK_STATE_NORMAL,
    TICK_STATE_ACCELERATE,
};

#define TICK_THRESHOLD 300
#define TICK_BASE 3

enum EndPointState
{
    ENDPOINT_NAVIGATE,
    ENDPOINT_RUN,
    ENDPOINT_EDIT,
};

class EndPoint
{
protected:
    uint8_t index = 0;
    uint8_t command = 0;
    uint8_t parameter = 0;
    uint8_t tickCounter = 0;
    EndPointState state = ENDPOINT_NAVIGATE;

private:
    unsigned long last = 0;

public:
    EndPoint()
    {
        last = millis();
    }
    virtual ~EndPoint() {}

    virtual void Setup(uint8_t index, uint8_t command, uint8_t parameter);
    virtual uint8_t GetIndex() { return index; }
    virtual void SetIndex(uint8_t i) { index = i; }
    virtual uint8_t GetCommand() { return command; }
    virtual void SetCommand(uint8_t c) { command = c; }
    virtual uint8_t GetParameter() { return parameter; }
    virtual void SetParameter(uint8_t p) { parameter = p; }

    virtual void Start() = 0;
    virtual bool Process(UserEvent event);

    virtual preset_base Get() = 0;
    virtual preset_base High() = 0;
    virtual void Set(preset_base) = 0;
    virtual char *Format(char *buffer, size_t bufLen)
    {
        return PresetBaseFormat(buffer, bufLen, Get());
    }

    preset_base tickFactor();
};
