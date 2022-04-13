// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "Menu.h"
Menu rootMenu("rootMenu", 4);

Menu::Menu(const char *label,
           uint8_t length,
           EndPointState hook,
           uint8_t range,
           EndPoint *endPoint) : label(label), length(length),
                                 state(state), range(range), endPoint(endPoint)
{
    if (length > 0)
    {
        nodes = (Menu **)malloc(sizeof(Menu *) * length);
    }
    else
    {
        range = 0;
    }
}

Menu::~Menu()
{
    if (nodes != NULL)
    {
        free(nodes);
    }
}

const char *Menu::Label()
{
    static char buf[16];
    if (range < 2)
    {
        return label;
    }
    snprintf(buf, sizeof(buf), "%s-%u", label, sequence);
    return buf;
}

Menu *Menu::Add(Menu *item)
{
    uint8_t newLevel = level + 1;
    if (count < length && newLevel < DEFAULT_STACK_DEPTH)
    {
        item->level = newLevel;
        nodes[count] = item;
        count++;
        limit += item->range;
    }
    return item;
}

Menu *Menu::Selection()
{
    if (count > 0)
    {
        uint8_t nx = 0;    // nodes index
        uint8_t accum = 0; // accumulator
        uint8_t seq = 0;   // node seq
        Menu *node;
        do
        {
            node = nodes[nx];
            seq = index - accum;
            accum += node->range;
            if (index < accum)
            {
                node->sequence = seq;
                return node;
            }
        } while (++nx < length);
    }
    return this;
}

Menu *Menu::AddExit()
{
    static const char *menuExit = "<-Back";
    return Add(new Menu(menuExit, 0, ENDPOINT_BACK));
}
