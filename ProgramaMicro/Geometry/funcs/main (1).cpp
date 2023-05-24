#include <cstdlib>
#include <iostream>

#include "Arc.h"

int main()
{
    Arc arc{
        {3.0, 0.0},
        {0.0, 3.0},
        {0.0, 0.0}
    };

    std::cout << arc.length() << '\n';

    return EXIT_SUCCESS;
}
