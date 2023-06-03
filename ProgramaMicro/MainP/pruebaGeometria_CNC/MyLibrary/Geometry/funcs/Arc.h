#ifndef ARC_H
#define ARC_H

#include <cmath>
#include <vector>

#include "Point.h"

class Arc
{
    public:
        Arc(Point a, Point b, Point c);
        Arc(Point a, Point b, double r);

        double length() const;
        std::vector<Point> sliceArc(double spacing) const;


    private:
        Point a_;
        Point b_;
        double r_;
};

#endif
