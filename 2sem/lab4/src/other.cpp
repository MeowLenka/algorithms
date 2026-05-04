#include "other.hpp"
#include <limits>
#include <cmath>


Point::Point(double x, double y) : _x(x), _y(y) {}

double Point::get_x() const { return _x; }

double Point::get_y() const { return _y; }

Circle::Circle(double radius) : _radius(radius), _center(Point(0, 0)) {}

Circle::Circle(double radius, Point center) : _radius(radius), _center(center) {}

bool Circle::operator==(const Circle &other) const
{
    // todo: учесть машинный epsilon 
    // параметры равны, если отличаются не больше чем на 2.22e-16
    bool r = std::fabs(_radius - other._radius) <= std::numeric_limits<double>::epsilon();
    bool x = std::fabs(_center.get_x() - other._center.get_x()) <= std::numeric_limits<double>::epsilon();
    bool y = std::fabs(_center.get_y() - other._center.get_y()) <= std::numeric_limits<double>::epsilon();
   return x && y && r;
}