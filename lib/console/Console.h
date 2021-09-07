// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <Arduino.h>
#include "ServoServe.h"

class Console
{
private:
    Stream &stream;
    ServoServe &serve;
    servo_cmd_t cmd;
    bool cmd_ready = false;

public:
    Console(Stream &stream,
            ServoServe &serve) : stream(stream),
                                 serve(serve) {}
    ~Console() {}

    bool ready();
    bool get();
    void parse(const char *cmd);
};
