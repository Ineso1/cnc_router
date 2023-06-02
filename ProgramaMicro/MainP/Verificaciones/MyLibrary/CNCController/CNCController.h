#ifndef CNCCONTROLLER_H
#define CNCCONTROLLER_H

#include <MKL25Z4.h>
#include "Stepper.h"
#include "math.h"
#include "UartCom.h"


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
        
        double x_up_duration;
        double x_dw_duration;

        double y_up_duration;
        double y_dw_duration;

        double z_up_duration;
        double z_dw_duration;

        int clk_frequen{41'940'000};

        int x_number_operations(double up, double down) { return (up + down)*clk_frequen; }
        int y_number_operations(double up, double down) { return (up + down)*clk_frequen; }
        int z_number_operations(double up, double down) { return (up + down)*clk_frequen; }

        mutable int x_coefficient;
        mutable int y_coefficient;
        mutable int z_coefficient;

        void num_sync(
            int x_num_op,
            int y_num_op,
            int z_num_op,
            int max_distance
        ) const
        {
            x_coefficient = max_distance / x_num_op;
            y_coefficient = max_distance / y_num_op;
            z_coefficient = max_distance / z_num_op; 
        }

        // Result is x_coefficient * x_num_op for each element in the tuple.
        // They must stop blinking at the same time.

    public:
        Stepper motorX;
        Stepper motorY;
        Stepper motorZ;
        Uart Serial;

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