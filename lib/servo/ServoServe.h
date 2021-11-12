// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include <ServoEasing.h>

#define SERVO_FIRST 1
#define SERVO_HOME 1
#define SERVO_MOVE 2
#define SERVO_EASE 3
#define SERVO_TEST 4
#define SERVO_STOP 5
#define SERVO_LAST 5

#define ERR_SERVO_OK 0
#define ERR_SERVO_NOT_ENOUGH_ARGS 1
#define ERR_SERVO_NOT_FOUND 2
#define ERR_SERVO_INDEX 3
#define ERR_SERVO_BAD_VALUE 4

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
    void start();
    void test(int angle);
    int process(const char *buf);
};
