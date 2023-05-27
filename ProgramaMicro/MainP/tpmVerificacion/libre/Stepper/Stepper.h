#ifndef STEPPER_H
#define STEPPER_H

#include <MKL25Z4.H>
#include <cstdint>
#include <stdint.h>
#include "MklTime.h"
#include "TpmMod.h"

const float PI = 3.1415926535897;
class Stepper{

    /*PINOUT*/
        int8_t pin; // Pin connected to the STEP signal of the stepper motor driver
        char pinPort;
        int8_t direction; // Pin connected to the direction signal of the stepper motor driver
        char directionPort;
    
    /*PROPS*/
        int steps; // Number of steps per revolution of the stepper motor
        float radius; // Radius of the pulley or gear attached to the stepper motor
        int position;    // Current position of the stepper motor
        int pulseDelay = 200;
    /*TPM*/
        TpmMod tpm;


    public:
        Stepper();
        void setPins(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmN, int channel); //Pin de puerto E
        char getPinPort();
        int8_t getPin();
        void setDirection(bool direction); //   1:positive   0:negative
        int getPosition();  // Getter function to retrieve the current position
        void useTpm();  //set port to tpm
        void quitTpm();
        void move_mm(float);
        void move_mm(float distance, bool tmp); //use tpm
        void setPulseDelay(int);
        int calculatePeriod(int delayTime);
        int calculatePulseDuration(int period);
        int calculateSteps_mm(float distance);
        void setTpmMod(int mod);
        void setChValue(int value);
        void setTpmPrescaler(int value);

        void enableTpm();

        void clrPulseCounter();
        void addPulseCounter();
        int getPulseCounter();
        void setPulsesTarget(int stepsNum);

        bool tpmProcess();

    private:
        void positiveDirection();
        void negativeDirection();
        void pulse();
};

#endif //STEPPER_H
