#include <iostream>
#include <cstdlib>
#include <vector>
#include "Point.h"
#include "Arc.h"
#include <cmath>

void moveDiagonal(const Point& end) {
    Point currentPos(0, 0, 0);

    // Calculate the distance between the start and end points
    double distanceX = end.x() - currentPos.x();
    double distanceY = end.y() - currentPos.y();
    double distanceZ = end.z() - currentPos.z();

    // Determine the maximum distance among the axes
    double maxDistance = std::abs(distanceX);
    if (std::abs(distanceY) > maxDistance)
        maxDistance = std::abs(distanceY);
    if (std::abs(distanceZ) > maxDistance)
        maxDistance = std::abs(distanceZ);

    // Calculate the step sizes for each axis
    double stepX = distanceX / maxDistance;
    double stepY = distanceY / maxDistance;
    double stepZ = distanceZ / maxDistance;

    // Move the CNC in steps along the diagonal line
    for (double i = 0; i <= maxDistance; i++) {
        double x = currentPos.x() + stepX * i;
        double y = currentPos.y() + stepY * i;
        double z = currentPos.z() + stepZ * i;
        std::cout << "x: " << x << ", y: " << y << ", z: " << z << '\n';
    }
}


int main(){
    // Arc arc{
    //     {30.0, 0.0, 0.0},
    //     {0.0, 26.45, 0.0},
    //     {40},
    // };
    // std::cout << arc.length() << '\n';

    // double spacing = 1.0; // Spacing between line segments (in mm)
    // std::vector<Point> slices = arc.sliceArc(spacing);

    // std::cout << "Sliced Arc Points:\n";
    // for (const auto& point : slices) {
    //     std::cout << "x: " << point.x() << ", y: " << point.y() << ", z: " << point.z() << '\n';
    // }


    // Define the starting and ending points of the diagonal
    Point end(1000, 73, 0);

    // Move the CNC along the diagonal line
    moveDiagonal(end);



    return EXIT_SUCCESS;
}