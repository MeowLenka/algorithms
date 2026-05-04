#include "circle_func.hpp"
#include <cmath>

namespace CircleFunc
{
    double circle_square(double radius) { return M_PI * radius * radius; }

    double circle_perimeter(double radius) { return 2 * M_PI * radius; }

}