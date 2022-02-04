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

    virtual void Write(const char *heading, const char *label);
    virtual void Write(const char *heading, const char *label,
                       preset_base value, preset_base high);
};

#endif