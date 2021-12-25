// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "UserEvent.h"

template <class T>
class ValueEditor
{
private:
    T *value = NULL;

public:
    ValueEditor() {}
    ~ValueEditor() {}

    virtual void Setup(T *v)
    {
        value = v;
    }

    virtual void Update() = 0;
    virtual void Previous() = 0;
    virtual void Select() = 0;
};
