// Copyright (c) 2022 Dave Marsh. See LICENSE.

#pragma once
#include "UserEvent.h"

class EventHandler
{
public:
    EventHandler( ) {}
    virtual ~EventHandler() {}

    virtual UserEvent Handle(UserEvent event) = 0;
    virtual bool Run() = 0;
};
