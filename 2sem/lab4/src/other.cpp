#include "other.hpp"


Point::Point(double x, double y) : _x(x), _y(y) {}

double Point::get_x() const { return _x; }

double Point::get_y() const { return _y; }



Circle::Circle(double radius) : _radius(radius), _center(Point(0, 0)) {}

Circle::Circle(double radius, Point center) : _radius(radius), _center(center) {}

bool Circle::operator==(const Circle &other) const
{
    return _radius == other._radius &&
           _center.get_x() == other._center.get_x() &&
           _center.get_y() == other._center.get_y();
}