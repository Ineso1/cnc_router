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


std::vector<Point> Arc::sliceArc(double spacing) const
{
    std::vector<Point> slices;

    // Obtener el ángulo central del arco
    double central_angle = 2 * std::asin(a_.distance(b_) / (2 * r_));

    // Calcular el número de puntos de corte
    int num_slices = static_cast<int>(std::ceil(central_angle / (spacing / r_)));

    // Calcular el ángulo de separación entre puntos
    double angular_spacing = central_angle / num_slices;

    // Calcular el ángulo inicial del arco
    double start_angle = std::atan2(a_.y(), a_.x());

    for (int i = 0; i <= num_slices; ++i) {
        double angle = start_angle + i * angular_spacing;

        // Calcular las coordenadas x e y del punto en el arco
        double x = r_ * std::cos(angle);
        double y = r_ * std::sin(angle);

        // Crear un punto en el arco y agregarlo a la lista de puntos
        Point point(x, y, 0);
        slices.push_back(point);
    }

    return slices;
}




