#ifndef POINT_H
#define POINT_H

#include <cmath>

class Point
{
    public:
        Point(double x, double y);
        Point() = default;
        Point(const Point& p) = default;

        Point& operator=(const Point& p);

        ~Point() = default;

        double distance(const Point p = {0.0, 0.0}) const;

        double x() const;
        double y() const;

        void x(const double x);
        void y(const double y);

    private:
        double x_{0.0};
        double y_{0.0};
};

#endif
