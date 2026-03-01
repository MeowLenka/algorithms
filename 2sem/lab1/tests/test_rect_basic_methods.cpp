#include <cassert>
#include <iostream>
#include "../src/rect.hpp"

int main()
{
    {
        Rect zero_rect;
        assert((zero_rect.get_left() == 0 && zero_rect.get_right() == 0 && zero_rect.get_bottom() == 0 && zero_rect.get_top() == 0));
    }
    
    {
        Rect zero_rect;
        zero_rect.set_all(10, 11, 13, 12);
        assert(zero_rect.get_left() == 10);
        assert(zero_rect.get_right() == 11);
        assert(zero_rect.get_top() == 13);
        assert(zero_rect.get_bottom() == 12);
    }

    {
        Rect rect(1, 1, 1, 1);
        assert((rect.get_left() == 1 && rect.get_right() == 1 && rect.get_bottom() == 1 && rect.get_top() == 1));
    }

    {
        Rect other(1, 2, 4, 1);
        Rect copy_rect(other);
        assert((copy_rect.get_left() == 1 && copy_rect.get_right() == 2 && copy_rect.get_bottom() == 1 && copy_rect.get_top() == 4));
    }

    return 0;
}