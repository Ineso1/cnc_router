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
        int8_t direction; // Pin connected to the direction signal of the stepper motor driver
        char directionPort;
        int steps; // Number of steps per revolution of the stepper motor
        float radius; // Radius of the pulley or gear attached to the stepper motor
        int position;    // Current position of the stepper motor
        int pulseDelay = 200;
        int tpm;

    public:
        Stepper();
        void setPins(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmNum); //Pin de puerto E
        char getPinPort();
        int8_t getPin();
        void setDirection(bool direction);
        int getPosition();  // Getter function to retrieve the current position
        void useTpm();  //set port to tpm
        void quitTpm();
        void move_mm(float);
        void move_mm(float distance, bool tmp); //use tpm
        void setPulseDelay(int);
        int calculatePeriod(int delayTime);
        int calculatePulseDuration(int period);
        int calculateSteps_mm(float distance);

    private:
        void positiveDirection();
        void negativeDirection();
        void pulse();
};

#endif //STEPPER_H
