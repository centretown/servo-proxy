// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "Menu.h"

Stack<Menu> Menu::stack(MAX_LEVELS);
Menu *Menu::Current() { return stack.Current(); }
Menu *Menu::Ancestor(uint8_t gen) { return stack.Ancestor(gen); }
Menu *Menu::Root(uint8_t gen) { return stack.Root(gen); }
EndPoint *Menu::point = NULL;
uint8_t Menu::state = MENU_NAVIGATE;

Menu rootMenu("", 4);
char Menu::route[ROUTESIZE] = {0};

EventReader *Menu::reader = NULL;
ResponseWriter *Menu::writer = NULL;

void Menu::SetReader(EventReader *r)
{
    reader = r;
}

void Menu::SetWriter(ResponseWriter *w)
{
    writer = w;
}

void Menu::SetPoint(EndPoint *p)
{
    point = p;
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

Menu *Menu::navigate(UserEvent event)
{
    switch (event)
    {
    case USER_PREVIOUS:
        return Previous();
    case USER_NEXT:
        return Next();
    case USER_SELECT:
        return Select();
    default:
        break;
    }
    return NULL;
}

void Menu::loop()
{
    UserEvent event = reader->GetEvent();
    if (event == USER_NOP)
    {
        return;
    }

    Menu *menu;

    if (state == MENU_EDIT)
    {
        menu = Current();
        if (point != NULL)
        {
            if (!point->process(event))
            {
                state = MENU_RUN;
                point->start();
            }
        }
    }
    else
    {
        menu = navigate(event);
    }

    if (writer == NULL)
    {
        return;
    }

    Menu *node = menu->Selection();

    if (state == MENU_EDIT)
    {
        char buf[18] = {0};
        snprintf(buf, sizeof(buf), "%s *", node->Label());
        point->SetParameter(menu->Index());
        writer->write(menu->Label(),
                      buf,
                      point->GetCounter());
        return;
    }
    else if (state == MENU_RUN)
    {
        point->SetParameter(menu->Index());
        writer->write(menu->Label(),
                      node->Label(),
                      point->GetCounter());
        return;
    }

    writer->write(menu->Label(), node->Label());
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

Menu *Menu::Next()
{
    Menu *menu = Current();
    menu->index++;
    if (menu->index >= menu->limit)
    {
        menu->index = 0;
    }
    return menu;
}
Menu *Menu::Previous()
{
    Menu *menu = stack.Current();
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

Menu *Menu::Select()
{
    Menu *menu = Current();
    Menu *node = menu->Selection();
    if (node->endpoint != NULL)
    {
        state = (state == MENU_NAVIGATE) ? MENU_RUN : MENU_EDIT;

        node->endpoint(node);

        if (node->count == 0)
        {
            return menu;
        }
    }

    if (node->count > 0)
    {
        stack.Push(node);
    }
    else //exit
    {
        state = (state == MENU_EDIT) ? MENU_RUN : MENU_NAVIGATE;

        menu->Next();
        stack.Pop();
    }
    return Current();
}

Menu::Menu(const char *label,
           void (*func)(Menu *),
           uint8_t length) : label(label), length(length), range(1)
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
           uint8_t range, void (*func)(Menu *)) : label(label), length(length), range(range)
{
    if (length > 0)
    {
        nodes = (Menu **)malloc(sizeof(Menu *) * length);
    }
    endpoint = func;
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
