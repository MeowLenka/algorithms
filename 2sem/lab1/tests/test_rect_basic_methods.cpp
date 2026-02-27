#include <cassert>
#include "lab1/src/rect.hpp"

int main()
{
    Rect zero_rect;
    assert((!zero_rect.get_left() && !zero_rect.get_right() && !zero_rect.get_bottom() && !zero_rect.get_height()) && "сторона не равна 0!");
    zero_rect.set_all(10, 11, 12, 13);
    assert((zero_rect.get_left() == 10));
    assert((zero_rect.get_right() == 11));
    assert((zero_rect.get_top() == 12));
    assert((zero_rect.get_bottom() == 13));

    Rect rect(1, 1, 1, 1);
    assert((zero_rect.get_left() == 1 && zero_rect.get_right() == 1 && zero_rect.get_bottom() == 1 && zero_rect.get_height() == 1));

    Rect other(1, 2, 3, 4);
    Rect copy_rect(other);
    assert((zero_rect.get_left() == 1 && zero_rect.get_right() == 2 && zero_rect.get_bottom() == 3 && zero_rect.get_height() == 4));


    return 0;
}