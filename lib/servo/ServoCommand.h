// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

enum ServoCommand
{
    SERVO_NOP,               // do nothing quickly
    SERVO_HOME,              // hardware home
    SERVO_MOVE,              // move at fixed rate
    SERVO_EASE,              // move at variable rate
    SERVO_TEST,              // continuous test
    SERVO_STOP,              // reset to blank
    SERVO_OUT_OF_BOUNDS,     // too big
    SERVO_FIRST = SERVO_HOME // command offset
};

typedef struct
{
    uint8_t command = 0;
    uint8_t angle = 0;
    uint8_t speed = 45;
    uint8_t type = 0;
    uint8_t fresh = 0;
} servo_cmd_t;

typedef struct
{
    uint8_t angle = 0;
    uint8_t speed = 45;
    uint8_t type = 0;
    uint8_t toggle = true;
} servo_preset_t;
