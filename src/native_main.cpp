// Copyright (c) 2021 Dave Marsh. See LICENSE.

#ifndef ARDUINO
#include <stdio.h>
#include "Menu.h"
#define USE_SERVO_LIB
#define USE_LEDSTRIP_LIB
// #define USE_ROTARY_LIB

void servoParmsEndPoint(Menu *menu)
{
}

void servoEndPoint(Menu *menu)
{
}

int8_t expanderPins[] = {2, 3, 5, 6};

#include "config_menus.h"

// void displayMenu(Menu *menu)
// {
//     printf("%*s%s l=%u c=%u\n",
//            menu->Level() * 4, " ", menu->Label(), menu->Level(), menu->Count());
//     // menu->EndPoint();
//     for (unsigned i = 0; i < menu->Count(); i++)
//     {
//         if (menu->Node(i) != NULL)
//         {
//             displayMenu(menu->Node(i));
//         }
//     }
// }

int main(int argc, char **argv)
{

    initServoMenu();
    initLedMenu();

    Menu::Start();
    // displayMenu(Menu::Root(0));
    // show(Menu::Current());
    unsigned option;
    do
    {
        show(Menu::Current());
        printf("1-previous 2-next 3-select 4-exit-> ");
        int count = scanf("%u", &option);
        if (count > 0)
        {
            switch (option)
            {
            case 1:
                Menu::Previous();
                break;
            case 2:
                Menu::Next();
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