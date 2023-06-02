#include "Point.h"

Point::Point(double x, double y, double z) : x_{x}, y_{y}, z_{z}
{
    
}

Point& Point::operator=(const Point& p)
{
    x_ = p.x_;
    y_ = p.y_;
    z_ = p.z_;

    return *this;
}

double Point::distance(const Point p) const
{
    double dx = p.x_ - x_;
    double dy = p.y_ - y_;
    double dz = p.z_ - z_;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

double Point::x() const
{
    return x_;
}

double Point::y() const
{
    return y_;
}

double Point::z() const
{
    return z_;
}

void Point::x(const double x)
{
    x_ = x;
}

void Point::y(const double y)
{
    y_ = y;
}

void Point::z(const double z)
{
    z_ = z;
}
