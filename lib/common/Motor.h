// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

template <class T>
class Motor
{
private:
    T *data = NULL;

public:
    Motor(T *data) : data(data) {}
    virtual ~Motor() {}

    virtual void Stop() {}
    virtual void Start() = 0;
    virtual void Tick() = 0;
};
