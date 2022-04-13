// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#ifdef ARDUINO

#include <Arduino.h>

#else

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <malloc.h>
#include <string.h>

#endif

#define DEFAULT_STACK_DEPTH 12

template <class T>
class Stack
{
private:
    uint8_t stackDepth = 0;
    T **stack = NULL;
    uint8_t stackPtr = 0;

public:
    Stack(uint8_t depth = DEFAULT_STACK_DEPTH) : stackDepth(depth)
    {
        if (depth > 0)
        {
            stack = (T **)malloc(sizeof(T *) * depth);
        }
    }
    ~Stack()
    {
        free(stack);
    }

    uint8_t StackPtr() { return stackPtr; }

    void Push(T *m)
    {
        if (stackPtr >= stackDepth - 1)
        {
            return;
        }
        stack[stackPtr] = m;
        stackPtr++;
    }

    void Pop()
    {
        if (stackPtr > 0)
        {
            --stackPtr;
        }
    }

    T *Current()
    {
        uint8_t i = (stackPtr > 0) ? stackPtr - 1 : 0;
        return stack[i];
    }

    T *Ancestor(uint8_t gen)
    {
        if (stackPtr >= gen)
        {
            return stack[stackPtr - gen];
        }
        return stack[0];
    }

    T *Root(uint8_t gen = 0)
    {
        if (gen >= stackPtr)
        {
            return stack[0];
        }
        return stack[gen];
    }
};