// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <base.h>

class ResponseWriter
{
public:
    ResponseWriter() {}
    ~ResponseWriter() {}

    virtual void write(const char *heading, const char *label) {}
    virtual void write(const char *heading, const char *label, int16_t value) {}
};
