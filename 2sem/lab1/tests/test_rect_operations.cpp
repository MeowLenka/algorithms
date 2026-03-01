// `move` и `inflate`
#include <iostream>
#include <cassert>
#include "../src/rect.hpp"

int main()
{
    {
        Rect rect(10, 20, 30, 10);
        rect.inflate(100);
        assert((rect.get_left() == -90 && rect.get_right() == 120 && rect.get_top() == 130 && rect.get_bottom() == -90));
    }

    {
        Rect rect;
        rect.inflate(10, 10);
        assert((rect.get_left() == -10 && rect.get_right() == 10 && rect.get_bottom() == -10 && rect.get_top() == 10));
    }

    {
        Rect rect(10, 20, 30, 10);
        rect.inflate(1, 2, 3, 4);
        assert((rect.get_left() == 9 && rect.get_right() == 22 && rect.get_top() == 33 && rect.get_bottom() == 6));
    }

    {
        Rect rectM(1, 2, 2, 1);
        rectM.move(5, 5);
        assert((rectM.get_left() == 6 && rectM.get_right() == 7 && rectM.get_top() == 7 && rectM.get_bottom() == 6));
    }

    {
        Rect rectM(1, 2, 2, 1);
        rectM.move(-5);
        assert((rectM.get_left() == -4 && rectM.get_right() == -3 && rectM.get_top() == 2 && rectM.get_bottom() == 1));
    }

    {
        Rect rectM(1, 2, 2, 1);
        rectM.move();
        assert((rectM.get_left() == 1 && rectM.get_right() == 2 && rectM.get_top() == 2 && rectM.get_bottom() == 1));
    }
    return 0;
}
