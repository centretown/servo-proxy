// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "EndPoint.h"
#include "ServoCommand.h"

class ServoEndPoint : public EndPoint
{
private:
    uint8_t value;
    servo_cmd_t cmd;

public:
    ServoEndPoint();
    ~ServoEndPoint() {}
    virtual void Setup(Menu *);
    virtual uint8_t Value(Menu *m);
    virtual void Exit(Menu *);
};