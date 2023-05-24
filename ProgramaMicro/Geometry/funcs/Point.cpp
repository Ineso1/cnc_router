#include "Point.h"

Point::Point(double x, double y) : x_{x}, y_{y}
{
    
}

Point& Point::operator=(const Point& p)
{
    x_ = p.x_;
    y_ = p.y_;
    return *this;
}

double Point::distance(const Point p) const
{
    return std::hypot(p.x_ - x_, p.y_ - y_);
}

double Point::x() const
{
    return x_;
}

double Point::y() const
{
    return y_;
}

void Point::x(const double x)
{
    x_ = x;
}

void Point::y(const double y)
{
    y_ = y;
}
