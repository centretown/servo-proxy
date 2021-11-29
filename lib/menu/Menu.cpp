// Copyright (c) 2021 Dave Marsh. See LICENSE.

#include "Menu.h"

Menu rootMenu("root");

Menu *Menu::stack[MAX_LEVELS] = {NULL};
uint8_t Menu::stackPtr = 0;

void Menu::push(Menu *m)
{
    if (stackPtr >= MAX_LEVELS - 1)
    {
        // printf("MAX_EXCEEDED\n");
        return;
    }
    stack[stackPtr] = m;
    stackPtr++;
}

void Menu::pop()
{
    // printf("POP\n");
    if (stackPtr > 0)
    {
        --stackPtr;
    }
}

Menu *Menu::Current()
{
    uint8_t i = (stackPtr > 0) ? stackPtr - 1 : 0;
    // printf("C=%u\n", i);
    // Menu *m = stack[i];
    // if (m == NULL)
    // {
    //     printf("CURRENT MENU NULL");
    // }
    return stack[i];
}

Menu *Menu::Ancestor(uint8_t gen)
{
    if (stackPtr >= gen)
    {
        return stack[stackPtr - gen];
    }
    return stack[0];
}

void Menu::Start()
{
    push(&rootMenu);
}

void Menu::Next()
{
    Menu *menu = Current();
    menu->index++;
    if (menu->index >= menu->count)
    {
        menu->index = 0;
    }
}

void Menu::Previous()
{
    Menu *menu = Current();
    if (menu->index == 0)
    {
        menu->index = menu->count - 1;
        return;
    }
    menu->index--;
}

void Menu::Select()
{
    // printf("SELECTing...\n");
    Menu *menu = Current();
    if (menu == NULL)
    {
        // printf("SELECT NULL MENU\n");
        return;
    }

    menu = menu->nodes[menu->index];
    if (menu == NULL)
    {
        pop();
        return;
    }

    if (menu->endpoint != NULL)
    {
        // printf("endpoint\n");
        menu->endpoint(menu);
        return;
    }

    if (menu->Count() > 0)
    {
        push(menu);
    }
    else //exit
    {
        // printf("POPPING...\n");
        pop();
    }
}

Menu::Menu(const char *text, void (*func)(Menu *))
{
    strncpy(label, text, sizeof(label));
    endpoint = func;
}

Menu::~Menu()
{
}

Menu *Menu::Add(Menu *item)
{
    if (count < MAX_NODES &&
        (level < MAX_LEVELS || level == UNSELECTED))
    {
        item->level = level + 1;
        nodes[count] = item;
        count++;
    }

    return item;
}