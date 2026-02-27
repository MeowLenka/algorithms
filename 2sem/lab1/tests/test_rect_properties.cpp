// void inflate(int amount); // отодвигает каждую из сторон от центра
#include <cassert>
#include "lab1/src/rect.hpp"

int main()
{
    // `get_width`, `get_height`, `get_square`, `set_width` и `set_height`
    Rect rect1(1, 5, 5, 1);
    assert((rect1.get_width() == 4 && rect1.get_height() == 4));
    rect1.set_height(10);
    assert((rect1.get_top() == 11 && rect1.get_bottom() ==  1));
    rect1.set_width(20);
    assert((rect1.get_left() ==  1 && rect1.get_right() == 21));
    assert(rect1.get_square() == 200);

    return 0;
}