// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#pragma once

#include "base.h"
#include "EventReader.h"

class NativeReader : public EventReader
{
public:
    NativeReader() {}
    ~NativeReader() {}

    virtual UserEvent GetEvent();
    
};

#endif // NOT ARDUINO