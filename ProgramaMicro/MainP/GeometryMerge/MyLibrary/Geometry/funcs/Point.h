#ifndef POINT_H
#define POINT_H

#include <cmath>

class Point
{
    public:
        Point(double x, double y, double z);
        Point() = default;
        Point(const Point& p) = default;

        Point& operator=(const Point& p);

        ~Point() = default;

        double distance(const Point p = {0.0, 0.0, 0.0}) const;

        double x() const;
        double y() const;
        double z() const;


        void x(const double x);
        void y(const double y);
        void z(const double z);


    private:
        double x_{0.0};
        double y_{0.0};
        double z_{0.0};

};

#endif
