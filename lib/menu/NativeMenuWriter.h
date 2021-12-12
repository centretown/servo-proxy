// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifndef ARDUINO
#pragma once

#include "base.h"
#include "MenuWriter.h"

class NativeMenuWriter : public MenuWriter
{
public:
    NativeMenuWriter() {}
    ~NativeMenuWriter() {}

    virtual void write(const char *heading, const char *label,
                       int32_t *value = NULL);
};

#endif