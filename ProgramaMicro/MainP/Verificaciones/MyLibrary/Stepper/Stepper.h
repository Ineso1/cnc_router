#ifndef STEPPER_H
#define STEPPER_H

#include <MKL25Z4.H>
#include <cstdint>
#include <stdint.h>
#include "MklTime.h"

const float PI = 3.1415926535897;
class Stepper{
        int8_t pin; // Pin connected to the STEP signal of the stepper motor driver
        char pinPort;
        int8_t enable; // Pin connected to the ENABLE signal of the stepper motor driver
        char enablePort;
        int steps; // Number of steps per revolution of the stepper motor
        float radius; // Radius of the pulley or gear attached to the stepper motor
        int position;    // Current position of the stepper motor
        int pulseDelay = 200;

    public:
        Stepper();
        void setPins(char pinPort, int pin, char enablePort, int enable, int steps, int radius); //Pin de puerto E
        char getPinPort();
        int8_t getPin();
        void disableMotor();
        void enableMotor();
        int getPosition();  // Getter function to retrieve the current position
        void useTpm();
        void quitTpm();
        void move_mm(float);
        void move_mm(float distance, int delayTime);
        void setPulseDelay(int);
        int calculatePeriod(int delayTime);
        int calculatePulseDuration(int period);

    private:
        void pulse();
};

#endif //STEPPER_H
