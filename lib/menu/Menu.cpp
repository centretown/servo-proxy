// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "Menu.h"

Stack<Menu> Menu::stack(MAX_LEVELS);
Menu *Menu::Current() { return stack.Current(); }
Menu *Menu::Ancestor(uint8_t gen) { return stack.Ancestor(gen); }
Menu *Menu::Root(uint8_t gen) { return stack.Root(gen); }

Menu rootMenu("", 4);
char Menu::route[ROUTESIZE] = {0};

MenuReader *Menu::reader = NULL;
MenuWriter *Menu::writer = NULL;

void Menu::SetReader(MenuReader *r)
{
    reader = r;
}

void Menu::SetWriter(MenuWriter *w)
{
    writer = w;
}

void Menu::setup()
{
    stack.Push(&rootMenu);
    if (writer != NULL)
    {
        Menu *menu = Current();
        Menu *node = menu->Selection();
        writer->write(menu->Label(), node->Label());
    }
}

void Menu::loop()
{
    MenuEvent event = MENU_NOP;
    if (reader == NULL)
    {
        return;
    }

    event = reader->GetEvent();
    switch (event)
    {
    case MENU_PREVIOUS:
        Previous();
        break;
    case MENU_NEXT:
        Next();
        break;
    case MENU_SELECT:
        Select();
        break;
    case MENU_NOP:
        return;
    }

    if (writer != NULL)
    {
        Menu *menu = Current();
        Menu *node = menu->Selection();
        writer->write(menu->Label(), node->Label());
    }
}

const char *Menu::Path()
{
    route[0] = 0;
    uint8_t stackPtr = stack.StackPtr();
    for (uint8_t i = 0; i < stackPtr; i++)
    {
        Menu *m = Root(i);
        strcat(route, m->Label());
        strcat(route, "/");
    };
    return route;
}

void Menu::Next()
{
    Menu *menu = Current();
    menu->index++;
    if (menu->index >= menu->limit)
    {
        menu->index = 0;
    }
}
void Menu::Previous()
{
    Menu *menu = stack.Current();
    if (menu->index == 0)
    {
        menu->index = menu->limit - 1;
        return;
    }
    menu->index--;
}

void Menu::Select()
{
    Menu *menu = Current();
    if (menu == NULL)
    {
        return;
    }

    Menu *node = menu->Selection();
    if (node->endpoint != NULL)
    {
        node->endpoint(node);
        if (node->count == 0)
        {
            return;
        }
    }

    if (node->count > 0)
    {
        stack.Push(node);
    }
    else //exit
    {
        menu->Next();
        stack.Pop();
    }
}

Menu::Menu(const char *label,
           void (*func)(Menu *)) : label(label), length(0), range(1)
{
    endpoint = func;
}

Menu::Menu(const char *label,
           uint8_t length) : label(label), length(length), range(1)
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

Menu::Menu(const char *label,
           uint8_t length,
           uint8_t range) : label(label), length(length), range(range)
{
    if (length > 0)
    {
        nodes = (Menu **)malloc(sizeof(Menu *) * length);
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
    if (count < length && newLevel < MAX_LEVELS)
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
        uint8_t n = 0;     // nodes index
        uint8_t accum = 0; // accumulator
        uint8_t seq = 0;   // node seq
        do
        {
            Menu *node = nodes[n];
            seq = index - accum;
            accum += node->range;
            if (index < accum)
            {
                node->sequence = seq;
                return node;
            }
        } while (++n < length);
    }
    return this;
}
