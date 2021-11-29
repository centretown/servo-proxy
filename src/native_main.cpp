// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#include <stdio.h>
#include "Menu.h"

Menu servoMenu("Servos");
Menu ledMenu("LED'S");
uint8_t servoPos = 0;

void endPoint(Menu *menu)
{
    char buf[80];
    snprintf(buf, sizeof(buf), "endpoint <%s i=%u l=%u c=%u>",
             menu->Label(), menu->Index(), menu->Level(), menu->Count());
    printf("%s\n", buf);
    uint8_t level = menu->Level();

    for (uint8_t i = 0; i < level; i++)
    {
        menu = menu->Ancestor(i);
        snprintf(buf, sizeof(buf), "endpoint <%s i=%u l=%u c=%u>",
                 menu->Label(), menu->Index(), menu->Level(), menu->Count());
        printf("%s\n", buf);
    }
}

void initServoMenu()
{
    char title[16];
    rootMenu.Add(&servoMenu);
    for (size_t servoNum = 0; servoNum < 4; servoNum++)
    {
        snprintf(title, sizeof(title), "Servo-%lu", servoNum + 1);
        Menu *sub = servoMenu.Add(new Menu(title));
        sub->Add(new Menu("Home", endPoint));
        sub->Add(new Menu("Move", endPoint));
        sub->Add(new Menu("Ease", endPoint));
        sub->Add(new Menu("Test", endPoint));
        sub->Add(new Menu("Stop", endPoint));
        sub->Add(new Menu("Exit"));
    }
    servoMenu.Add(new Menu("Exit"));
}
void initLedMenu()
{
    char title[16];
    rootMenu.Add(&ledMenu);
    for (size_t servoNum = 0; servoNum < 4; servoNum++)
    {
        snprintf(title, sizeof(title), "LED-%lu", servoNum + 1);
        Menu *sub = ledMenu.Add(new Menu(title));
        sub->Add(new Menu("Clear", endPoint));
        sub->Add(new Menu("Solid", endPoint));
        sub->Add(new Menu("Blink", endPoint));
        sub->Add(new Menu("Rainbow", endPoint));
        sub->Add(new Menu("Theatre", endPoint));
        sub->Add(new Menu("Exit"));
    }
    ledMenu.Add(new Menu("Exit"));
}

void displayMenu(Menu *menu)
{
    char tabs[32] = {0};

    if (menu->Level() != UNSELECTED)
    {
        memset(tabs, ' ', 2 * (menu->Level() + 1));
    }
    printf("%s%s l=%u c=%u\n", tabs, menu->Label(), menu->Level(), menu->Count());
    // menu->EndPoint();
    for (unsigned i = 0; i < menu->Count(); i++)
    {
        if (menu->nodes[i] != NULL)
        {
            displayMenu(menu->nodes[i]);
        }
    }
}

void show(Menu *menu)
{
    if (menu == NULL)
    {
        printf("null\n");
        return;
    }
    uint8_t index = menu->Index();
    if (index != UNSELECTED)
    {
        menu = menu->nodes[index];
        printf("%s\n", menu->Label());
    }
}

int main(int argc, char **argv)
{

    initServoMenu();
    initLedMenu();

    Menu::Start();
    displayMenu(&rootMenu);

    unsigned option;
    do
    {
        show(Menu::Current());
        printf("1-next 2-previous 3-select 4-exit\n");
        int count = scanf("%u", &option);
        if (count > 0)
        {
            switch (option)
            {
            case 1:
                Menu::Next();
                break;
            case 2:
                Menu::Previous();
                break;
            case 3:
                Menu::Select();
                break;
            default:
                break;
            }
        }
    } while (option != 4);
}

#endif