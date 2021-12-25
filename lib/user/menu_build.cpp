
#include "Menu.h"

void addExit(Menu *menu)
{
    menu->Add(new Menu("Exit"));
}

void addEndpoints(Menu *menu,
                  const char *labels[], size_t count,
                  void (*endpoint)(Menu *))
{
    for (size_t i = 0; i < count; i++)
    {
        menu->Add(new Menu(labels[i], endpoint));
    }
    // addExit(menu);
}

void addEndpoints(Menu *menu,
                  const char *labels[], size_t count,
                  const char *parms[], size_t parmsCount,
                  void (*endpoint)(Menu *), void (*editor)(Menu *))
{
    Menu *sub;
    for (size_t i = 0; i < count; i++)
    {
        sub = menu->Add(new Menu(labels[i], parmsCount + 1, 1, endpoint));
        addEndpoints(sub, parms, parmsCount, editor);
        addExit(sub);
    }
}

void addMenus(Menu *menu,
              const char *labels[], size_t count, size_t length,
              void (*func)(Menu *))
{
    Menu *sub;
    for (size_t i = 0; i < count; i++)
    {
        sub = menu->Add(new Menu(labels[i], length));
        if (func != NULL)
        {
            func(sub);
        }
    }
    addExit(menu);
}
