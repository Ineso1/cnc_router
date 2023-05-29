#include "Arc.h"

Arc::Arc(Point a, Point b, Point c) : a_{a}, b_{b}, r_{a.distance(c)}
{

}

Arc::Arc(Point a, Point b, double r) : a_{a}, b_{b}, r_{r}
{

}

double Arc::length() const
{
    return r_*std::acos(
        1 - std::pow(a_.distance(b_), 2)/(2*std::pow(r_, 2))
    );
}
