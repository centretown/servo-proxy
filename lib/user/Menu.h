// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "Stack.h"
#include "EventReader.h"
#include "ResponseWriter.h"
#include "EndPoint.h"

class Navigator;

class Menu
{
private:
    const char *label;
    uint8_t length = 0; // number of nodes allocated
    EndPointState state = ENDPOINT_NAV;
    uint8_t range = 0; // number indeces filled by this node
    EndPoint *endPoint;

    uint8_t level = 0;    // menu depth
    uint8_t limit = 0;    // number of indeces added
    uint8_t count = 0;    // number of nodes added
    uint8_t index = 0;    // current node
    uint8_t sequence = 0; // current index within node

    Menu **nodes = NULL;

    friend Navigator;

public:
    Menu(const char *label, uint8_t length = 1,
         EndPointState state = ENDPOINT_NAV,
         uint8_t range = 1, EndPoint *point = NULL);
    ~Menu();

public:
    const char *Label();
    uint8_t Count() { return count; }
    Menu *Selection();
    Menu *Node(uint8_t n) { return nodes[n]; }
    Menu *Add(Menu *item);
    Menu *AddExit();
};

extern Menu rootMenu;
