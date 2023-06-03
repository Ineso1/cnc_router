/**
 * @file Stepper.h
 * @brief Header file for the Stepper class.
 */

#ifndef STEPPER_H
#define STEPPER_H

#include <MKL25Z4.H>
#include <cstdint>
#include <stdint.h>
#include "MklTime.h"
#include "TpmMod.h"

const float PI = 3.1415926535897;
class Stepper{
    private:    
        
        /*PINOUT*/
        int8_t pin; // Pin connected to the STEP signal of the stepper motor driver
        char pinPort;
        int8_t direction; // Pin connected to the direction signal of the stepper motor driver
        char directionPort;
        
        /*PROPS*/
        int steps; // Number of steps per revolution of the stepper motor
        float radius; // Radius of the pulley or gear attached to the stepper motor
        int position;    // Current position of the stepper motor
        int pulseDelay = 300;
        
        /*TPM*/
        TpmMod tpm;
        
        void positiveDirection();
        void negativeDirection();
        void pulse();

    public:
        Stepper();

        /*....Setters..........................................................*/
        void setPins(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmN, int channel); //Pin de puerto E
        void setDirection(bool direction); //   1:positive   0:negative
        void setTpmMod(int mod);
        void setChValue(int value);
        void setPulsesTarget(int stepsNum);
        void setPulseDelay(int);
        void setTpmPrescaler(int value);
        void clrPulseCounter();
        void addPulseCounter();
        void substractPulseCounter();

        /*....Getter................................................................*/
        char getPinPort();
        int8_t getPin();
        int getPosition();  // Getter function to retrieve the current position
        int getPulseCounter();
        int getPulseTarget();
        bool tpmProcess();

        /*....enables-diables.......................................................*/
        void useTpm();  //set port to tpm
        void quitTpm();
        void enableTpm();
        
        /*....Movement..............................................................*/
        void move_mm(float);
        void moveTo(int target);
        void moveTo(int target, int mod, int channelVal, int Npulses);
        
        /*....Calculation...........................................................*/
        int calculatePeriod(int delayTime);         //Pendiente de revison
        int calculatePulseDuration(int period);     //Pendiente de revison
        int calculateSteps_mm(float distance); 

    
};

#endif //STEPPER_H
