// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO
#pragma once

#include <Arduino.h>
#include <ServoEasing.h>

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

enum ServoCommand
{
    SERVO_NOP,  // 0-do nothing quickly
    SERVO_HOME, // 2-hardware home
    SERVO_MOVE, // 3-move at fixed rate
    SERVO_EASE, // 4-move at variable rate
    SERVO_TEST, // 5-continuous test
    SERVO_STOP, // 1-reset to blank
    SERVO_OUT_OF_BOUNDS
};

typedef struct
{
    uint8_t command = 0;
    uint8_t angle = 0;
    uint8_t speed = 45;
    uint8_t type = 0;
    uint8_t fresh = 0;
} servo_cmd_t;

class ServoServe
{
private:
    ServoEasing **servos;
    servo_cmd_t commands[16];
    const int8_t *expanderPins;
    const size_t count;
    void attach(ServoEasing *servo, int expanderPin);
    bool ready = false;
    uint8_t testAngle = 0;

public:
    ServoServe(
        ServoEasing **servos,
        int8_t *expanderPins,
        size_t count) : servos(servos),
                        expanderPins(expanderPins),
                        count(count) {}

    ~ServoServe() {}

    void setup();
    void loop();
    int start(uint8_t index, uint8_t command, uint8_t angle, uint8_t speed, uint8_t type);
    void test(int angle);
    int process(const char *buf);
};
#endif