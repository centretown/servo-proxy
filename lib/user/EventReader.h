// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

#include "UserEvent.h"

class EventReader
{
public:
    virtual UserEvent GetEvent() = 0;
};
