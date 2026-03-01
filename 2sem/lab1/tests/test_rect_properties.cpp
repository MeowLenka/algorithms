#include <cassert>
#include "../src/rect.hpp"

int main()
{
    Rect rect1(1, 5, 5, 1);
    assert((rect1.get_width() == 4 && rect1.get_height() == 4));

    rect1.set_height(10);
    assert((rect1.get_top() == 11 && rect1.get_bottom() ==  1 && rect1.get_height() == 10));

    rect1.set_width(20);
    assert((rect1.get_left() ==  1 && rect1.get_right() == 21 && rect1.get_width() == 20));

    assert(rect1.get_square() == 200);

    return 0;
}