// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "UserEvent.h"

class EndPoint
{
protected:
    uint8_t index = 0;
    uint8_t command = 0;
    uint8_t parameter = 0;
    uint8_t increment = 1;
    uint64_t last = 0;

public:
    EndPoint() { last = millis(); }
    ~EndPoint() {}

    virtual void setup(uint8_t i, uint8_t c, uint8_t p);
    uint8_t GetIndex() { return index; }
    void SetIndex(uint8_t i) { index = i; }
    uint8_t GetCommand() { return command; }
    void SetCommand(uint8_t c) { command = c; }
    uint8_t GetParameter() { return parameter; }
    void SetParameter(uint8_t p) { parameter = p; }

    virtual void start() {}
    virtual bool process(UserEvent event);

    virtual int16_t GetCounter() = 0;
    virtual void SetCounter(int16_t) = 0;
};
