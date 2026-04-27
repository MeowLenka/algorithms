#ifndef OTHER_HPP
#define OTHER_HPP

class Point
{
    double _x, _y;

public:
    double get_x() const;
    double get_y() const;
    Point(double x = 0, double y = 0);
};


class Circle
{
public:
    Point _center;
    double _radius;

    Circle(double radius = 0);
    Circle(double radius, Point center);

    bool operator==(const Circle &other) const;
};

#endif
