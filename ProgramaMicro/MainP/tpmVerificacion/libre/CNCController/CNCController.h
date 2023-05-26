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

        int maxStep(int a, int b, int c);
        int calculatePrescaler(int steps, int maxSteps);
        int calculatePrescaler(int steps, int maxSteps, int delay);
        int calculateCompareValue(int steps, int maxSteps, int prescaler);

    public:
        CNCController();
        void setPinX(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        void setPinY(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        void setPinZ(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        
        void moveX(float distance); 
        void moveY(float distance);
        void moveZ(float distance);

        void moveTo(float x, float y, float z);

        void setPrescalerCompare(uint16_t prescalerX, uint16_t compareX, uint16_t prescalerY, uint16_t compareY, uint16_t prescalerZ, uint16_t compareZ);
};

#endif // CNCCONTROLLER_H