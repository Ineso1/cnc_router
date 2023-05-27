#include "CNCController.h"

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
    __enable_irq();

    motorX.useTpm();
    motorY.useTpm();
    motorZ.useTpm();

    motorX.clrPulseCounter();
    motorY.clrPulseCounter();
    motorZ.clrPulseCounter();

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

    // Calculate the absolute number of steps for each motor
    stepsX = abs(stepsX);
    stepsY = abs(stepsY);
    stepsZ = abs(stepsZ);

    // Find the maximum number of steps among all motors
    int maxSteps = maxStep(stepsX, stepsY, stepsZ);

    // // Calculate the module and compare values based on the maximum steps
    uint16_t compareX = calculateCompareValue(stepsX, maxSteps);
    uint16_t compareY = calculateCompareValue(stepsY, maxSteps);
    uint16_t compareZ = calculateCompareValue(stepsZ, maxSteps);
    
    motorX.setPulsesTarget(10);
    motorY.setPulsesTarget(10);
    motorZ.setPulsesTarget(10);

    // Set the prescaler and compare values for each TPM (Timer/Counter)
    setPrescalerCompare(compareX, compareY, compareZ);

    // Enable TPMs
    motorX.enableTpm();
    motorY.enableTpm();
    motorZ.enableTpm();

    while (!(motorX.tpmProcess() && motorY.tpmProcess() && motorZ.tpmProcess())) {}
    motorX.quitTpm();
    motorY.quitTpm();
    motorZ.quitTpm();

    motorX.clrPulseCounter();
    motorY.clrPulseCounter();
    motorZ.clrPulseCounter();

    __disable_irq();
}

void CNCController::setPrescalerCompare( uint16_t compareX, uint16_t compareY, uint16_t compareZ) {
  // Configure the TPM module with the specified prescaler and compare value
    motorX.setTpmPrescaler(128);
    motorY.setTpmPrescaler(128);
    motorZ.setTpmPrescaler(128);

    motorX.setChValue(30000);
    motorY.setChValue(30000);
    motorZ.setChValue(30000);

    motorX.setTpmMod(60000);
    motorY.setTpmMod(60000);
    motorZ.setTpmMod(60000);

}

// Function to calculate the compare value based on the number of steps, maximum steps, and prescaler
int CNCController::calculateCompareValue(int steps, int maxSteps) {
    // Calculate the desired number of pulses per step
    int pulsesPerStep = 1000; // Adjust this value based on the requirements

    // Calculate the maximum counter value for the TPM
    int maxCounter = 0xFFFF; // Assuming 16-bit TPM

    // Calculate the counter value
    int counter = (steps * pulsesPerStep) / (maxSteps + 1);

    // Adjust the counter value to fit within the valid range
    if (counter > maxCounter) {
        counter = maxCounter;
    } else if (counter < 1) {
        counter = 1;
    }

    return counter;
}

