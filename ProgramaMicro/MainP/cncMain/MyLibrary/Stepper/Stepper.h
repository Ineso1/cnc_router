#ifndef MOVEMOTOR_H
#define MOVEMOTOR_H

#include <MKL25Z4.H>
#include <stdint.h>
#include "MklTime.h"

const float PI = 3.1415926535897;
class Stepper{
        int8_t pin; // Pin connected to the STEP signal of the stepper motor driver
        int8_t enable; // Pin connected to the ENABLE signal of the stepper motor driver
        int steps; // Number of steps per revolution of the stepper motor
        float radius; // Radius of the pulley or gear attached to the stepper motor

    public:
        void setPins(int, int, int, int); //Pin de puerto E
        void disableMotor();
        void enableMotor();
        void move_mm(float);


    private:
        void pulse();
};

#endif
