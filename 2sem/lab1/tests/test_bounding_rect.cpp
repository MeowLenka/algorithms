// `move` и `inflate`
#include <iostream>
#include <cassert>
#include "../src/rect.hpp"

int main()
{
    {
        Rect rect1(2, 4, 4, 2);
        Rect rect2(3, 6, 6, 3);
        Rect bound_rect = bounding_rect(rect1, rect2);
        assert((bound_rect.get_left() == 2 && bound_rect.get_right() == 6 && bound_rect.get_bottom() == 2 && bound_rect.get_top() == 6));
    }
    
    try
    {
        Rect rectW(1, 2, 3, 4);
    }
    catch (const char *e)
    {
        assert(strcmp(e, "wrong rect") == 0);
    }

    try
    {
        Rect rectW(4, 3, 2, 1);
    }
    catch (const char *e)
    {
        assert(strcmp(e, "wrong rect") == 0);
    }

    try
    {
        Rect rectW(10, 1, 1, 10);
    }
    catch (const char *e)
    {
        assert(strcmp(e, "wrong rect") == 0);
    }

    return 0;
}