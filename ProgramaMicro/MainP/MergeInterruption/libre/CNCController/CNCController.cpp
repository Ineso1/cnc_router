#include "CNCController.h"
#include <string>

CNCController::CNCController() {
    // Initialize position variables
    motorXPosition = 0;
    motorYPosition = 0;
    motorZPosition = 0;
}

void CNCController::setPinX(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmN, int channel, int stopSwitch){
    motorX.setPins(pinPort, pin, directionPort, direction, steps, radius, tpmN, channel);  // Example values, adjust according to your setup
    stopSwitchX = stopSwitch;
}

void CNCController::setPinY(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmN, int channel, int stopSwitch){
    motorY.setPins(pinPort, pin, directionPort, direction, steps, radius, tpmN, channel);  // Example values, adjust according to your setup
    stopSwitchY = stopSwitch;
}

void CNCController::setPinZ(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmN, int channel, int stopSwitch){
    motorZ.setPins(pinPort, pin, directionPort, direction, steps, radius, tpmN, channel);  // Example values, adjust according to your setup
    stopSwitchZ = stopSwitch;
}

/**
 * @brief Move the CNC along the X-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveX(float distance) {
    motorZ.quitTpm();
    motorY.quitTpm();
    motorX.move_mm(distance);  // Pass nullptr for unused axes
}

/**
 * @brief Move the CNC along the Y-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveY(float distance) {
    motorZ.quitTpm();
    motorX.quitTpm();
    motorY.move_mm(distance);  // Pass nullptr for unused axes
}

/**
 * @brief Move the CNC along the Z-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveZ(float distance) {
    motorY.quitTpm();
    motorX.quitTpm();
    motorZ.move_mm(distance);  // Pass nullptr for unused axes
}

int CNCController::maxStep(int a, int b, int c) {
  if (a >= b && a >= c) {
    return a;
  } else if (b >= a && b >= c) {
    return b;
  } else {
    return c;
  }
}

void CNCController::moveTo(float x, float y, float z){
    __disable_irq();

    float pulseDuration = 0.0002;  // in milliseconds
    float minDelayBetween = 0.0003;
    int prescaler = 128;

    // Calculate the number of steps required to reach the target position
    int stepsX = motorX.calculateSteps_mm(x);
    int stepsY = motorY.calculateSteps_mm(y);
    int stepsZ = motorZ.calculateSteps_mm(z);
 
    // Determine the direction for each motor based on the target position
    int directionX = (stepsX >= 0) ? 1 : 0;
    int directionY = (stepsY >= 0) ? 1 : 0;
    int directionZ = (stepsZ >= 0) ? 1 : 0;

    motorX.setDirection(directionX);
    motorY.setDirection(directionY);
    motorZ.setDirection(directionZ);

    // Find the maximum number of steps among all motors
    int maxSteps = maxStep(stepsX, stepsY, stepsZ);

    uint16_t moduleValue, channelValue;

    // // Calculate the module and compare values based on the maximum steps
    calculateModuleAndChannelValues( prescaler, pulseDuration, minDelayBetween, maxSteps, moduleValue, channelValue);

    uint32_t XmoduleValue, YmoduleValue, ZmoduleValue;
    XmoduleValue = moduleValue * ( maxSteps / static_cast<float>(stepsX));
    YmoduleValue = moduleValue * ( maxSteps / static_cast<float>(stepsY));
    ZmoduleValue = moduleValue * ( maxSteps / static_cast<float>(stepsZ));

    motorX.moveTo(x, XmoduleValue, channelValue, stepsX);
    motorY.moveTo(y, YmoduleValue, channelValue, stepsY);
    // motorX.moveTo(x, 0xFFFF, 10);
    // motorY.moveTo(y, 0XFFFF/2, 20);
    // motorZ.moveTo(z, 0XFFFF, 11);

    // Set the module and compare values for each TPM (Timer/Counter)

    __enable_irq();
    // Enable TPMs
    motorX.enableTpm();
    motorY.enableTpm();
    motorZ.enableTpm();

    while (!(motorX.tpmProcess() && motorY.tpmProcess() && motorZ.tpmProcess())) {}

}

void CNCController::setPrescalerCompare( uint16_t compareX, uint16_t compareY, uint16_t compareZ) {
  // Configure the TPM module with the specified prescaler and compare value
    // motorX.setTpmPrescaler(128);
    // motorY.setTpmPrescaler(128);
    // motorZ.setTpmPrescaler(128);

}

void CNCController::calculateModuleAndChannelValues(int prescaler, float pulseDuration, float minDelayBetween, int maxSteps, uint16_t& moduleValue, uint16_t& channelValue) {
    
    float frequency = 41940000;

    // Calculate the pulse frequency
    float timeInstruction = maxSteps * (pulseDuration + minDelayBetween);  // Convert pulse duration to seconds

    // Calculate the module value
    moduleValue = static_cast<uint32_t>(std::round( timeInstruction * ( frequency / prescaler)));

    // Calculate the channel value
    channelValue = ((frequency * pulseDuration ) / prescaler);
}