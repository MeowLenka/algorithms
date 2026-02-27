// `move` и `inflate`
#include <cassert>
#include "lab1/src/rect.hpp"

int main()
{
    Rect rect;
    rect.inflate(10, 10);
    assert((rect.get_left() == -10 && rect.get_right() == 10 && rect.get_bottom() == -10 && rect.get_top() == 10));
    return 0;
}
// DRY(dont repeat)  KISS(write simple) YAGNI(dont write прозапас)