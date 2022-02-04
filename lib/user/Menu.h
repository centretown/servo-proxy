// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "Stack.h"
#include "EventReader.h"
#include "ResponseWriter.h"
#include "EndPoint.h"

#define MAX_LEVELS 12
#define ROUTESIZE 64

#define MENU_NAVIGATE 0x00
#define MENU_RUN 0x02
#define MENU_EDIT 0x03

class Menu
{
private:
    const char *label;
    uint8_t length = 0;   // number of nodes allocated
    uint8_t range = 0;    // number indeces filled by this node
    uint8_t level = 0;    // menu depth
    uint8_t limit = 0;    // number of indeces added
    uint8_t count = 0;    // number of nodes added
    uint8_t index = 0;    // current node
    uint8_t sequence = 0; // current index within node

    EndPoint *endpoint = NULL;
    Menu **nodes = NULL;

    static char route[ROUTESIZE];
    static EventReader *reader;
    static ResponseWriter *writer;
    static Stack<Menu> stack;
    static EndPoint *point;
    static uint8_t state;
    static Menu *navigate(UserEvent event);

public:
    Menu(const char *label, EndPoint *point = NULL, uint8_t length = 0);
    Menu(const char *label, uint8_t length);
    Menu(const char *label, uint8_t length,
         uint8_t range, EndPoint *point = NULL);
    ~Menu();

    static void SetReader(EventReader *);
    static void SetWriter(ResponseWriter *);
    static const char *Path();
    static Menu *Current();
    static Menu *Ancestor(uint8_t gen);
    static Menu *Root(uint8_t gen = 0);
    static void SetPoint(EndPoint *p);

    static Menu *Select();
    static Menu *Next();
    static Menu *Previous();

    static void loop();
    static void setup();

    const char *Label();
    uint8_t Index() { return index; }
    uint8_t Level() { return level; }
    uint8_t Count() { return count; }
    uint8_t Length() { return length; }
    uint8_t Sequence() { return sequence; }
    uint8_t Range() { return range; }

    Menu *Selection();
    Menu *Node(uint8_t n) { return nodes[n]; }

    Menu *Add(Menu *item);
    Menu *AddExit(EndPoint *point = NULL);
};

extern Menu rootMenu;
