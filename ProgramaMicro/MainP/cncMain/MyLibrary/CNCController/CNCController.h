#ifndef CNCCONTROLLER_H
#define CNCCONTROLLER_H

#include <MKL25Z4.h>
#include "Stepper.h"
#include "math.h"

class CNCController{
    private:

        float motorXPosition;  // Motor X position in millimeters
        float motorYPosition;  // Motor Y position in millimeters
        float motorZPosition;  // Motor Z position in millimeters

        // Stop switch pin numbers
        int stopSwitchX;
        int stopSwitchY;
        int stopSwitchZ;

        int maxStep(int a, int b, int c);
        void calculateModuleAndChannelValues(int prescaler, float pulseDuration, float minDelayBetween, int maxSteps, uint16_t& moduleValue, uint16_t& channelValue);


    public:
        Stepper motorX;
        Stepper motorY;
        Stepper motorZ;

        /* Setting */
        CNCController();
        void setPinX(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        void setPinY(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        void setPinZ(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        
        /* Motion */
        void moveX(float distance); 
        void moveY(float distance);
        void moveZ(float distance);

        void moveTo(float x, float y, float z);


        /*Pendientes*/
        void home();
        void moveArc(float centerX, float centerY, float endX, float endY, bool isClockwise);
        void setRelativePosition(float x, float y, float z);
        void initStopSwitches();
        bool isXStopSwitchPressed();
        bool isYStopSwitchPressed();
        bool isZStopSwitchPressed();
        float calculateRadius(float centerX, float centerY, float endX, float endY);
        float calculateAngle(float centerX, float centerY, float x, float y, float radius);



};

#endif // CNCCONTROLLER_H