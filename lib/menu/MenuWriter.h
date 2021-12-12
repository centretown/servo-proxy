// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include <base.h>

class MenuWriter
{
public:
    MenuWriter() {}
    ~MenuWriter() {}
    virtual void write(const char *heading, const char *label,
                       int32_t *value = NULL) = 0;
};
