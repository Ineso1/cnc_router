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
    motorX.quitTpm();
    motorX.move_mm(distance);  // Pass nullptr for unused axes
}

/**
 * @brief Move the CNC along the Y-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveY(float distance) {
    motorZ.quitTpm();
    motorY.quitTpm();
    motorX.quitTpm();
    motorY.move_mm(distance);  // Pass nullptr for unused axes
}

/**
 * @brief Move the CNC along the Z-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveZ(float distance) {
    motorZ.quitTpm();
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
    motorX.useTpm();
    motorY.useTpm();
    motorZ.useTpm();

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

    // // Calculate the prescaler and compare values based on the maximum steps
    // uint16_t prescalerX = calculatePrescaler(stepsX, maxSteps);
    // uint16_t compareX = calculateCompareValue(stepsX, maxSteps, prescalerX);
    // uint16_t prescalerY = calculatePrescaler(stepsY, maxSteps);
    // uint16_t compareY = calculateCompareValue(stepsY, maxSteps, prescalerY);
    // uint16_t prescalerZ = calculatePrescaler(stepsZ, maxSteps);
    // uint16_t compareZ = calculateCompareValue(stepsZ, maxSteps, prescalerZ);
    
    // Set the prescaler and compare values for each TPM (Timer/Counter)
    // setPrescalerCompare(prescalerX, compareX, prescalerY, compareY, prescalerZ, compareZ);

    // Enable TPMs
    motorX.enableTpm();
    motorY.enableTpm();
    motorZ.enableTpm();
}

void CNCController::setPrescalerCompare(uint16_t prescalerX, uint16_t compareX, uint16_t prescalerY, uint16_t compareY, uint16_t prescalerZ, uint16_t compareZ) {
  // Configure the TPM module with the specified prescaler and compare value
    motorX.setTpmMod(prescalerX);
    motorY.setTpmMod(prescalerY);
    motorZ.setTpmMod(prescalerZ);

    motorX.setTpmMod(compareX);
    motorY.setTpmMod(compareY);
    motorZ.setTpmMod(compareZ);

}

// Function to calculate the compare value based on the number of steps, maximum steps, and prescaler
int CNCController::calculateCompareValue(int steps, int maxSteps, int prescaler) {
    // Calculate the desired number of pulses per step
    int pulsesPerStep = 1000; // Adjust this value based on the requirements

    // Calculate the maximum counter value for the TPM
    int maxCounter = 0xFFFF; // Assuming 16-bit TPM

    // Calculate the counter value
    int counter = (steps * pulsesPerStep * prescaler) / (maxSteps + 1);

    // Adjust the counter value to fit within the valid range
    if (counter > maxCounter) {
        counter = maxCounter;
    } else if (counter < 1) {
        counter = 1;
    }

    return counter;
}

// Function to calculate the prescaler value based on the number of steps, maximum steps, and delay
int CNCController::calculatePrescaler(int steps, int maxSteps, int delay) {
    float timePerStep = delay / 1000.0f; // Convert delay to seconds
    float maxTime = timePerStep * maxSteps;
    float prescalerFloat = maxTime / steps / 0.00000004f; // Assuming 80 MHz system clock
    int prescaler = round(prescalerFloat);

    // Ensure the prescaler value is within the valid range
    if (prescaler < 0) {
        prescaler = 0;
    } else if (prescaler > 7) {
        prescaler = 7;
    }

    return prescaler;
}

// Function to calculate the prescaler value based on the number of steps and maximum steps
int CNCController::calculatePrescaler(int steps, int maxSteps) {
    
    // Calculate the desired number of pulses per step
    int pulsesPerStep = 1000; // Adjust this value based on the requirements

    // Calculate the maximum counter value for the TPM
    int maxCounter = 0xFFFF; // Assuming 16-bit TPM

    // Calculate the prescaler value
    int prescaler = (steps * pulsesPerStep) / (maxSteps + 1);

    // Adjust the prescaler to fit within the valid range
    if (prescaler > maxCounter) {
        prescaler = maxCounter;
    } else if (prescaler < 1) {
        prescaler = 1;
    }

    return prescaler;
}