// Copyright (c) 2021 Dave Marsh. See LICENSE.
#ifndef ARDUINO
#pragma once

#include "base.h"
#include "ResponseWriter.h"

class NativeWriter : public ResponseWriter
{
private:
    void writeMenu();

public:
    NativeWriter() {}
    ~NativeWriter() {}

    virtual void write(const char *heading, const char *label);
    virtual void write(const char *heading, const char *label, int16_t value);
};

#endif