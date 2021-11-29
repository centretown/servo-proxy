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

#define MAX_NODES 12
#define MAX_LEVELS 6
#define UNSELECTED 0xff

class Menu
{
private:
    char label[16] = {0};
    void (*endpoint)(Menu *) = NULL;

    uint8_t level = 0;
    uint8_t count = 0;
    uint8_t index = 0;

public:
    Menu(const char *label, void (*endpoint)(Menu *) = NULL);
    ~Menu();

    Menu *Add(Menu *item);
    static Menu *stack[MAX_LEVELS];
    static uint8_t stackPtr;
    static void push(Menu *);
    static void pop();

    static Menu *Current();
    static Menu *Ancestor(uint8_t gen);
    static Menu *Root();
    static void Select();
    static void Next();
    static void Previous();
    static void Start();

    const char *Label() { return label; }
    uint8_t Index() { return index; }
    uint8_t Level() { return level; }
    uint8_t Count() { return count; }
    void EndPoint()
    {
        if (endpoint != NULL)
        {
            endpoint(this);
        }
    }

    Menu *nodes[MAX_NODES] = {NULL};
};

extern Menu rootMenu;
