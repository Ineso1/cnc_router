#include "mbed.h"
#include "Stepper.h"

Stepper motor;
// main() runs in its own thread in the OS
int main()
{
    __enable_irq();
    motor.setPins('C', 1, 'C', 2, 2000, 10, 0, 0);
    motor.moveTo(1);
    while (true) {

    }
}

