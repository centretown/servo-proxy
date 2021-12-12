// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "base.h"
#include "Stack.h"
#include "MenuReader.h"
#include "MenuWriter.h"

// #define MAX_NODES 12
#define MAX_LEVELS 12
#define UNSELECTED 0xff
#define UNSEQUENCED 0xff
#define ROUTESIZE 64

class Menu
{
private:
    const char *label;
    uint8_t length = 0;   //number of nodes allocated
    uint8_t range = 0;    //number indeces filled by this node
    uint8_t level = 0;    //menu depth
    uint8_t limit = 0;    //number of indeces added
    uint8_t count = 0;    //number of nodes added
    uint8_t index = 0;    //current node
    uint8_t sequence = 0; //current index within node

    void (*endpoint)(Menu *) = NULL;
    // Editor *editor = NULL;
    Menu **nodes = NULL;

    static char route[ROUTESIZE];
    static MenuReader *reader;
    static MenuWriter *writer;
    static Stack<Menu> stack;

public:
    Menu(const char *label, void (*endpoint)(Menu *) = NULL);
    Menu(const char *label, uint8_t length);
    Menu(const char *label, uint8_t length, uint8_t range);
    ~Menu();

    Menu *Add(Menu *item);

    static void SetReader(MenuReader *);
    static void SetWriter(MenuWriter *);
    static const char *Path();
    static Menu *Current();
    static Menu *Ancestor(uint8_t gen);
    static Menu *Root(uint8_t gen = 0);

    static void Select();
    static void Next();
    static void Previous();

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
