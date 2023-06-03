#ifndef CNCCONTROLLER_H
#define CNCCONTROLLER_H

#include <MKL25Z4.h>
#include "Stepper.h"
#include "math.h"
#include "UartCom.h"
#include "Arc.h"
#include "Point.h"
#include <vector>


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
        void calculateModuleAndChannelValues(int prescaler, float pulseDuration, float minDelayBetween, int maxSteps, float& moduleValue, uint16_t& channelValue);
        
        float pulseDuration = (300e-6);     // in milliseconds
        float minDelayBetween = (400e-6);   // in milliseconds


        int clk_frequen{8'000'000};
        // int frequency = 41940000;  //main clock


    public:
        Stepper motorX;
        Stepper motorY;
        Stepper motorZ;
        Uart Serial;

        /*....Setting...............................................................*/
        CNCController();
        void setPinX(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        void setPinY(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        void setPinZ(char pinPort, int pin, char enablePort, int enable, int steps, int radius, int tpmN, int channel, int stopSwitch);
        
        /*....Motion................................................................*/
        void moveX(float distance); 
        void moveY(float distance);
        void moveZ(float distance);

        void moveTo(float x, float y, float z);
        void moveArc(const Point& finalPoint, const Point& center,  double spacing);
        void moveArc(const Point& finalPoint, double radius, double spacing);
        void moveDiagonal(const Point& end);



        /*....Pendientes............................................................*/
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