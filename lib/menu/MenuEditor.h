// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"

class Editor
{
private:
public:
    Editor() {}
    ~Editor() {}

    virtual void Setup() = 0;
    virtual void Next() = 0;
    virtual void Previous() = 0;
    virtual void Select() = 0;
};
