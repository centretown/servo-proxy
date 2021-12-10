// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO
#pragma once

#include <Arduino.h>
#include <ServoEasing.h>
#include "ServoCommand.h"

#if defined(SHARED_ERROR)
#define ERR_SERVO_OK ERR_OK
#define ERR_SERVO_NOT_ENOUGH_ARGS ERR_NOT_ENOUGH_ARGS
#define ERR_SERVO_NOT_FOUND ERR_NOT_FOUND
#define ERR_SERVO_INDEX ERR_INDEX
#define ERR_SERVO_BAD_VALUE ERR_BAD_VALUE
#else
#define ERR_SERVO_OK 0
#define ERR_SERVO_NOT_ENOUGH_ARGS 1
#define ERR_SERVO_NOT_FOUND 2
#define ERR_SERVO_INDEX 3
#define ERR_SERVO_BAD_VALUE 4
#endif

class ServoServe
{
private:
    ServoEasing **servos;
    const int8_t *expanderPins;
    const size_t count;

    servo_cmd_t *commands;
    servo_cmd_t *presets;

    void attach(ServoEasing *servo, int expanderPin);
    bool ready = false;
    uint8_t testAngle = 0;

public:
    ServoServe(
        ServoEasing **servos, int8_t *expanderPins, size_t count);

    ~ServoServe();

    void setup();
    void loop();
    int start(uint8_t index, uint8_t command, uint8_t angle, uint8_t speed, uint8_t type);
    int start(uint8_t index, servo_cmd_t *p);
    void test(int angle);
    int process(const char *buf);
};
#endif