// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO
#ifdef USE_SERVO_LIB

#pragma once

#include "ServoServe.h"
#include "EndPoint.h"

class ServoEndPoint : public EndPoint
{
private:
    ServoServe &servo;

public:
    ServoEndPoint(ServoServe &servo) : servo(servo) {}
    ~ServoEndPoint();

    virtual void start() {}
    virtual void setup(uint8_t i, uint8_t c, uint8_t p)
    {
        EndPoint::setup(i, c, p);
        // seg = strips.Segment(index);
    }
    virtual int16_t GetCounter() { return 0; }
    virtual void SetCounter(int16_t) {}
};

#endif // USE_SERVO_LIB
#endif // ARDUINO