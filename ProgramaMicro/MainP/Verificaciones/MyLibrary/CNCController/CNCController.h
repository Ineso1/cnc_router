#ifndef CNCCONTROLLER_H
#define CNCCONTROLLER_H

#include <MKL25Z4.h>
#include "Stepper.h"
#include "math.h"

class CNCController{
    private:
        Stepper motorX;
        Stepper motorY;
        Stepper motorZ;

        float motorXPosition;  // Motor X position in millimeters
        float motorYPosition;  // Motor Y position in millimeters
        float motorZPosition;  // Motor Z position in millimeters

        // Stop switch pin numbers
        int stopSwitchX;
        int stopSwitchY;
        int stopSwitchZ;

        bool isXStopSwitchPressed();
        bool isYStopSwitchPressed();
        bool isZStopSwitchPressed();

        void configureTPM();
        float calculateRadius(float centerX, float centerY, float endX, float endY);
        float calculateAngle(float centerX, float centerY, float x, float y, float radius);

        void enableTPM();
        void disableTPM();

        int calculateCompareValue(int steps, int maxSteps, int prescaler);
        int calculatePrescaler(int steps, int maxSteps, int delay);
        int calculatePrescaler(int steps, int maxSteps);
        int maxStep(int a, int b, int c);

        void setPrescalerCompare(uint16_t prescalerX, uint16_t compareX, uint16_t prescalerY, uint16_t compareY, uint16_t prescalerZ, uint16_t compareZ);

    public:
        CNCController();
        /*
        A:  PTA0 - TPM0_CH5
            PTA3 - TPM0_CH0
            PTA4 - TPM0_CH1
            PTA5 - TPM0_CH2
        B: 
        C:  PTC1 - TPM0_CH0
            PTC2 - TPM0_CH1
            PTC3 - TPM0_CH2
            PTC4 - TPM0_CH3
            PTC8 - TPM0_CH4
            PTC9 - TPM0_CH5
        D:  PTD0 - TPM0_CH0
            PTD1 - TPM0_CH1
            PTD2 - TPM0_CH2
            PTD3 - TPM0_CH3
            PTD4 - TPM0_CH4
            PTD5 - TPM0_CH5
        
        E:  PTE29 - TPM0_CH2
            PTE30 - TPM0_CH3
            PTE31 - TPM0_CH4
            PTE24 - TPM0_CH0
            PTE25 - TPM0_CH1
        */
        void setPinX(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int stopSwitch);
        /*
        A:  PTA12 - TPM1_CH0
            PTA13 - TPM1_CH1
        B:  PTB0 - TPM1_CH0
            PTB1 - TPM1_CH1
        C: 
        D: 
        E:  PTE20 - TPM1_CH0
            PTE21 - TPM1_CH1  
        */
        void setPinY(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int stopSwitch);
        /*
        A:  PTA1 - TPM2_CH0
            PTA2 - TPM2_CH1
        B:  PTB2 - TPM2_CH0
            PTB3 - TPM2_CH1 
            PTB18 - TPM2_CH0
            PTB19 - TPM2_CH1  
        C: 
        D: 
        E:  PTE22 - TPM2_CH0
            PTE23 - TPM2_CH1
        */
        void setPinZ(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int stopSwitch);

        void initStopSwitches();

        void moveX(float distance); 
        void moveY(float distance);
        void moveZ(float distance);

        void moveTo(float x, float y, float z);

        void moveArc(float centerX, float centerY, float endX, float endY, bool isClockwise);

        void home();
        void setRelativePosition(float x, float y, float z);
};

#endif // CNCCONTROLLER_H