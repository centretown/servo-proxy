// Copyright (c) 2022 Dave Marsh. See LICENSE.

#include "Navigator.h"

void Navigator::Setup()
{
    stack.Push(&rootMenu);
    Menu *menu = Current();
    Menu *node = menu->Selection();
    writer->Write(menu->Label(), node->Label());
}

void Navigator::Loop()
{
    UserEvent event = reader->GetEvent();
    if (event == USER_WAIT)
    {
        return;
    }

    Menu *menu =
        (state == ENDPOINT_EDIT) ? edit(event) : navigate(event);
    Menu *node = menu->Selection();
    if (state == ENDPOINT_NAV)
    {
        writer->Write(menu->Label(), node->Label());
    }
    else
    {
        writer->Write(menu->Label(), node->Label(),
                      endPoint->Get(), endPoint->High(),
                      state == ENDPOINT_EDIT);
    }
}

Menu *Navigator::edit(UserEvent event)
{
    if (endPoint->Edit(event))
    {
        state = ENDPOINT_RUN;
        endPoint->Start();
        stack.Pop();
    }
    return Current();
}

Menu *Navigator::navigate(UserEvent event)
{
    if (event == USER_NEXT)
    {
        return Next();
    }
    if (event == USER_PREVIOUS)
    {
        return Previous();
    }
    return Select();
}

Menu *Navigator::Next()
{
    Menu *menu = Current();
    menu->index++;
    if (menu->index >= menu->limit)
    {
        menu->index = 0;
    }
    return menu;
}
Menu *Navigator::Previous()
{
    Menu *menu = Current();
    if (menu->index == 0)
    {
        menu->index = menu->limit - 1;
    }
    else
    {
        menu->index--;
    }
    return menu;
}

Menu *Navigator::Select()
{
    Menu *menu = Current();
    Menu *node = menu->Selection();
    if (node->endPoint != NULL)
    {
        endPoint = node->endPoint;
    }

    state = node->state;
    endPoint->Select(state, menu->index);
    if (state == ENDPOINT_RUN)
    {
        endPoint->Start();
    }

    if (state != ENDPOINT_BACK && node->count > 0)
    {
        stack.Push(node);
    }
    else // exit
    {
        // menu->Next();
        stack.Pop();
    }
    return Current();
}
