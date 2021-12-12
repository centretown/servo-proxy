// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#pragma once

#include "base.h"
#include "MenuReader.h"

class NativeMenuReader : public MenuReader
{
public:
    NativeMenuReader() {}
    ~NativeMenuReader() {}

    virtual MenuEvent GetEvent();
    
};

#endif