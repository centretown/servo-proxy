// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifdef ARDUINO

#pragma once

#include <Arduino.h>

class CameraListener
{
private:
    Stream &stream;
    char buffer[81];
    size_t bufferCount = 0;
    bool commandReady = false;
    bool messageReady = false;

public:
    CameraListener(Stream &s) : stream(s) {}
    bool ready();
    bool hasCommand() { return commandReady; };
    bool hasMessage() { return messageReady; };
    char *get()
    {
        commandReady = messageReady = false;
        return buffer;
    }
};

#endif