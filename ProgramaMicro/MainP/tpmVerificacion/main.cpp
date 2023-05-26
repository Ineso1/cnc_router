#include <MKL25Z4.h>
#include "CNCController.h"

CNCController Control;

int main() {

    Control.setPinX('C', 1, 'C', 2, 2000, 10, 0, 0, 1);
    Control.setPinX('B', 1, 'B', 0, 2000, 10, 1, 1, 2);

    while (true) {
        Control.moveTo(100, 100, 100);
    }
}
