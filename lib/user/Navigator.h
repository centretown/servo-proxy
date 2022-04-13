// Copyright (c) 2021 Dave Marsh. See LICENSE.

#pragma once

#include "Menu.h"

#define STACK_DEPTH 12

class Navigator
{
private:
    EventReader *reader;
    ResponseWriter *writer;
    Stack<Menu> stack;
    EndPointState state = ENDPOINT_NAV;
    EndPoint *endPoint = NULL;

public:
    Navigator(
        EventReader *reader,
        ResponseWriter *writer) : reader(reader), writer(writer) {}
    ~Navigator() {}

    // void SetEndPoint(EndPoint *p) { endPoint = p; }

    Menu *Current() { return stack.Current(); }
    Menu *Ancestor(uint8_t gen) { return stack.Ancestor(gen); }
    Menu *Root(uint8_t gen = 0) { return stack.Root(gen); }

    Menu *Select();
    Menu *Next();
    Menu *Previous();

    void Loop();
    void Setup();

private:
    Menu *edit(UserEvent event);
    Menu *navigate(UserEvent event);
};
