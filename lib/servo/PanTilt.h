// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ServoEasing.h>

class PanTilt
{
private:
    ServoEasing *pan;
    ServoEasing *tilt;
    unsigned long next = 0;

public:
    PanTilt(ServoEasing *pan, ServoEasing *tilt) : pan(pan), tilt(tilt) {}
    ~PanTilt() {}

    void loop();

public:
};
