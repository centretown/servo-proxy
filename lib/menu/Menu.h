// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include "base.h"
#endif

#include "Stack.h"
#include "EndPoint.h"

// #define MAX_NODES 12
#define MAX_LEVELS 12
#define UNSELECTED 0xff
#define UNSEQUENCED 0xff
#define ROUTESIZE 64

class Menu
{
private:
    const char *label;
    uint8_t length = 0; //number of nodes allocated
    uint8_t range = 0;  //number indeces filled by this node
    uint8_t level = 0;  //menu depth
    uint8_t limit = 0;  //number of indeces added
    uint8_t count = 0;  //number of nodes added
    uint8_t index = 0;  //current
    uint8_t sequence = 0;  //current
    void (*endpoint)(Menu *) = NULL;
    EndPoint *point = NULL;
    Menu **nodes = NULL;

    static char route[ROUTESIZE];
    static Stack<Menu> stack;

public:
    Menu(const char *label, void (*endpoint)(Menu *) = NULL);
    Menu(const char *label, uint8_t length);
    Menu(const char *label, uint8_t length, uint8_t range);
    ~Menu();

    Menu *Add(Menu *item);

    static const char *Path();
    static Menu *Current();
    static Menu *Ancestor(uint8_t gen);
    static Menu *Root(uint8_t gen = 0);

    static void Select();
    static void Next();
    static void Previous();
    static void Start();

    const char *Label();
    uint8_t Index() { return index; }
    uint8_t Level() { return level; }
    uint8_t Count() { return count; }
    uint8_t Length() { return length; }
    uint8_t Sequence() { return sequence; }

    // Menu *Node(uint8_t i) { return nodes[i]; }
    Menu *Selection();

    uint8_t Range() { return range; }
};

extern Menu rootMenu;

void addExit(Menu *menu);
void addEndpoints(Menu *menu,
                  const char *text[], size_t textCount,
                  void (*endpoint)(Menu *));
void addMenus(Menu *menu,
              const char *text[], size_t textCount,
              size_t length,
              void (*endpoint)(Menu *));
