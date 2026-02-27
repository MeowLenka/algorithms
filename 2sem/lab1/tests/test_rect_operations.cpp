// `move` и `inflate`
#include <cassert>
#include "lab1/src/rect.hpp"

int main()
{
    Rect rect;
    rect.inflate(10, 10);
    assert((rect.get_left() == -10 && rect.get_right() == 10 && rect.get_bottom() == -10 && rect.get_top() == 10));

    Rect rect1(2, 4, 4, 2);
    Rect rect2(3, 6, 6, 3);
    Rect bound_rect = bounding_rect(rect1, rect2);
    assert((bound_rect.get_left() == 2 && bound_rect.get_right() == 6 && bound_rect.get_bottom() == 2 && bound_rect.get_top() == 6));


    return 0;
}
