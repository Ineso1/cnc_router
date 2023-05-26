#include "CNCController.h"

CNCController::CNCController() {
    // Initialize position variables
    motorXPosition = 0;
    motorYPosition = 0;
    motorZPosition = 0;
    configureTPM();
}

void CNCController::setPinX(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int stopSwitch){
    motorX.setPins(pinPort, pin, directionPort, direction, steps, radius, 0);  // Example values, adjust according to your setup
    stopSwitchX = stopSwitch;
}

void CNCController::setPinY(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int stopSwitch){
    motorY.setPins(pinPort, pin, directionPort, direction, steps, radius, 1);  // Example values, adjust according to your setup
    stopSwitchY = stopSwitch;
}

void CNCController::setPinZ(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int stopSwitch){
    motorZ.setPins(pinPort, pin, directionPort, direction, steps, radius, 2);  // Example values, adjust according to your setup
    stopSwitchZ = stopSwitch;
}


/**
 * @brief Initialize the stop switches for auto homing.
 */
void CNCController::initStopSwitches() {
    // Function implementation
}

/*Important!!!!!!!!!!*/
//To define port???
// bool CNCController::isXStopSwitchPressed() {
//     // Read the status of the stop switch for motor X
//     return (GPIO_ReadPinInput(GPIOX, stopSwitchX) == GPIO_PIN_LOW);
// }

// bool CNCController::isYStopSwitchPressed() {
//     // Read the status of the stop switch for motor Y
//     return (GPIO_ReadPinInput(GPIOX, stopSwitchY) == GPIO_PIN_LOW);
// }

// bool CNCController::isZStopSwitchPressed() {
//     // Read the status of the stop switch for motor Z
//     return (GPIO_ReadPinInput(GPIOX, stopSwitchZ) == GPIO_PIN_LOW);
// }


/**
 * @brief Move the CNC along the X-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveX(float distance) {
    disableTPM();
    motorZ.quitTpm();
    motorX.move_mm(distance);  // Pass nullptr for unused axes
}

/**
 * @brief Move the CNC along the Y-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveY(float distance) {
    disableTPM();
    motorZ.quitTpm();
    motorY.move_mm(distance);  // Pass nullptr for unused axes
}

/**
 * @brief Move the CNC along the Z-axis by a specified distance.
 * @param distance The distance to move in millimeters.
 */
void CNCController::moveZ(float distance) {
    disableTPM();
    motorZ.quitTpm();
    motorZ.move_mm(distance);  // Pass nullptr for unused axes
}


/**
 * @brief Enable the TPM module to start making changes.
 */
void CNCController::enableTPM() {
    TPM0->SC |= TPM_SC_CMOD(1);  // Enable TPM0 counter
    TPM1->SC |= TPM_SC_CMOD(1);
    TPM2->SC |= TPM_SC_CMOD(1);
}

/**
 * @brief Disable the TPM module to stop making changes.
 */
void CNCController::disableTPM() {
    TPM0->SC &= ~TPM_SC_CMOD(1);  // Disable TPM0 counter
    TPM1->SC &= ~TPM_SC_CMOD(1);
    TPM2->SC &= ~TPM_SC_CMOD(1);
}

/**
 * @brief Configure the TPM module for controlling the CNC motors.
 */
 /*---------------------------------------------------------------------------------------------
MSB stands for Most Significant Bit, and ELSB stands for Edge or Level Select Bit.

MSA and ELSA represent the bit positions for the MSB and ELSB in the TPM_CnSC register, respectively.

The values 1:0 in MSB:MSA and ELSB:ELSA indicate the bit settings for the corresponding bits.

1:0 means that both the MSB and ELSB bits are set to 1, indicating specific configurations for the TPM channel.

In the context of the TPM module configuration, the specific values 1:0 for MSB:MSA and ELSB:ELSA usually correspond to the following settings:
    *   MSB:MSA = 1:0: This configuration sets the TPM channel to use the TPM_CnV register in the center-aligned PWM mode.
    *   ELSB:ELSA = 1:0: This configuration sets the TPM channel to output a low-true pulse (active low) on the PWM signal.
---------------------------------------------------------------------------------------------*/
void CNCController::configureTPM() {
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // Enable TPM0 clock
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // Enable TPM1 clock
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;  // Enable TPM1 clock

    disableTPM();
    TPM0->SC = TPM_SC_PS(7);  // Prescaler = 128
    TPM0->MOD = 0xFFFF;       // Set the modulus to the maximum value
    
    TPM1->SC = TPM_SC_PS(7);  // Prescaler = 128
    TPM1->MOD = 0xFFFF;       // Set the modulus to the maximum value

    TPM2->SC = TPM_SC_PS(7);  // Prescaler = 128
    TPM2->MOD = 0xFFFF;       // Set the modulus to the maximum value

    // Configure TPM0 channel 0 as PWM output for motorX
    TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Enable edge-aligned PWM mode (MSB:MSA = 1:0, ELSB:ELSA = 1:0)
    TPM0->CONTROLS[0].CnV = 0;  // Set initial PWM value

    // Configure TPM1 channel 0 as PWM output for motorY
    TPM1->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Enable edge-aligned PWM mode (MSB:MSA = 1:0, ELSB:ELSA = 1:0)
    TPM1->CONTROLS[1].CnV = 0;  // Set initial PWM value

    // Configure TPM2 channel 0 as PWM output for motorZ
    TPM2->CONTROLS[1].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Enable edge-aligned PWM mode (MSB:MSA = 1:0, ELSB:ELSA = 1:0)
    TPM2->CONTROLS[1].CnV = 0;  // Set initial PWM value

    enableTPM();  // Enable TPM0 counter

}


/**
 * @brief Calculate the radius of an arc given the center and endpoint coordinates.
 * @param centerX The X-coordinate of the center point of the arc.
 * @param centerY The Y-coordinate of the center point of the arc.
 * @param endX The X-coordinate of the endpoint of the arc.
 * @param endY The Y-coordinate of the endpoint of the arc.
 * @return The radius of the arc.
 */
/*---------------------------------------------------------------------------------------------
Calculates the radius of an arc based on the center point and the endpoint.
Parameters:
centerX (float): X-coordinate of the center point.
centerY (float): Y-coordinate of the center point.
endX (float): X-coordinate of the endpoint.
endY (float): Y-coordinate of the endpoint.

Returns:
radius (float): The calculated radius of the arc.

Formula:
radius = sqrt((endX - centerX)^2 + (endY - centerY)^2)
---------------------------------------------------------------------------------------------*/
float CNCController::calculateRadius(float centerX, float centerY, float endX, float endY) {
    float dx = endX - centerX;
    float dy = endY - centerY;
    return sqrt(dx * dx + dy * dy);
}



/**
 * @brief Calculate the angle between a point on the arc and the center point.
 * @param centerX The X-coordinate of the center point of the arc.
 * @param centerY The Y-coordinate of the center point of the arc.
 * @param x The X-coordinate of a point on the arc.
 * @param y The Y-coordinate of a point on the arc.
 * @param radius The radius of the arc.
 * @return The angle in radians.
 */
/*---------------------------------------------------------------------------------------------
Calculates the angle between a point and the center of an arc.
Parameters:
centerX (float): X-coordinate of the center point.
centerY (float): Y-coordinate of the center point.
x (float): X-coordinate of the point.
y (float): Y-coordinate of the point.
radius (float): Radius of the arc.
Returns:
angle (float): The calculated angle between the point and the center of the arc.
Formula:
angle = atan2(y - centerY, x - centerX) * radius
---------------------------------------------------------------------------------------------*/
float CNCController::calculateAngle(float centerX, float centerY, float x, float y, float radius) {
    float dx = x - centerX;
    float dy = y - centerY;
    return atan2(dy, dx) * radius;
}



/**
 * @brief Move the CNC in an arc from the start point to the end point.
 * @param centerX The X-coordinate of the center point of the arc.
 * @param centerY The Y-coordinate of the center point of the arc.
 * @param endX The X-coordinate of the endpoint of the arc.
 * @param endY The Y-coordinate of the endpoint of the arc.
 * @param isClockwise Set to true for a clockwise arc, false for a counterclockwise arc.
 */
/*---------------------------------------------------------------------------------------------
Description: Moves the motors in an arc from the current position to the specified endpoint.
Parameters:
centerX (float): X-coordinate of the center point of the arc.
centerY (float): Y-coordinate of the center point of the arc.
endX (float): X-coordinate of the endpoint of the arc.
endY (float): Y-coordinate of the endpoint of the arc.
isClockwise (bool): Flag indicating whether the arc should be traversed in a clockwise direction.
Returns: None
Formula:
The function internally uses the calculateRadius and calculateAngle functions to determine the radius and angle of the arc.
It then iteratively calculates the intermediate positions along the arc using the calculated angle and moves the motors to those positions using the moveX and moveY functions.
---------------------------------------------------------------------------------------------*/
void CNCController::moveArc(float centerX, float centerY, float endX, float endY, bool isClockwise) {
    float radius = calculateRadius(centerX, centerY, endX, endY);
    float startAngle = calculateAngle(centerX, centerY, motorX.getPosition(), motorY.getPosition(), radius);
    float endAngle = calculateAngle(centerX, centerY, endX, endY, radius);
    float angleStep = isClockwise ? -0.1f : 0.1f;  // Adjust the angle step size according to your needs

    for (float angle = startAngle; isClockwise ? angle > endAngle : angle < endAngle; angle += angleStep) {
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);

        // Move the motors to the calculated position
        moveX(x);
        moveY(y);
    }

    // Move the motors to the final position
    moveX(endX);
    moveY(endY);
}


void CNCController::home() {
    // Move motor X towards the home position until the stop switch is pressed
    while (!isXStopSwitchPressed()) {
        moveX(-0.1f);  // Adjust the distance and direction 
    }
    motorXPosition = 0.0f;

    // Move motor Y towards the home position until the stop switch is pressed
    while (!isYStopSwitchPressed()) {
        moveY(-0.1f);  // Adjust the distance and direction 
    }
    motorYPosition = 0.0f;

    // Move motor Z towards the home position until the stop switch is pressed
    while (!isZStopSwitchPressed()) {
        moveZ(-0.1f);  // Adjust the distance and direction 
    }
    motorZPosition = 0.0f;
}

void CNCController::setRelativePosition(float x, float y, float z) {
    motorXPosition += x;
    motorYPosition += y;
    motorZPosition += z;
}


bool CNCController::isXStopSwitchPressed(){
    return true;
}
bool CNCController::isYStopSwitchPressed(){
    return true;
}
bool CNCController::isZStopSwitchPressed(){
    return true;
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
    int directionX = (stepsX >= 0) ? 1 : -1;
    int directionY = (stepsY >= 0) ? 1 : -1;
    int directionZ = (stepsZ >= 0) ? 1 : -1;

    // Calculate the absolute number of steps for each motor
    stepsX = abs(stepsX);
    stepsY = abs(stepsY);
    stepsZ = abs(stepsZ);

    // Find the maximum number of steps among all motors
    int maxSteps = maxStep(stepsX, stepsY, stepsZ);

    // Calculate the prescaler and compare values based on the maximum steps
    uint16_t prescalerX = calculatePrescaler(stepsX, maxSteps);
    uint16_t compareX = calculateCompareValue(stepsX, maxSteps, prescalerX);
    uint16_t prescalerY = calculatePrescaler(stepsY, maxSteps);
    uint16_t compareY = calculateCompareValue(stepsY, maxSteps, prescalerY);
    uint16_t prescalerZ = calculatePrescaler(stepsZ, maxSteps);
    uint16_t compareZ = calculateCompareValue(stepsZ, maxSteps, prescalerZ);
    
    // Set the prescaler and compare values for each TPM (Timer/Counter)
    setPrescalerCompare(prescalerX, compareX, prescalerY, compareY, prescalerZ, compareZ);

    // Enable TPMs
    enableTPM();
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

int CNCController::maxStep(int a, int b, int c) {
  if (a >= b && a >= c) {
    return a;
  } else if (b >= a && b >= c) {
    return b;
  } else {
    return c;
  }
}


void CNCController::setPrescalerCompare(uint16_t prescalerX, uint16_t compareX, uint16_t prescalerY, uint16_t compareY, uint16_t prescalerZ, uint16_t compareZ) {
  // Configure the TPM module with the specified prescaler and compare value
  TPM0->MOD = prescalerX;
  TPM1->MOD = prescalerY;
  TPM2->MOD = prescalerZ;

  TPM0->CONTROLS[0].CnV = compareX;
  TPM1->CONTROLS[0].CnV = compareY;
  TPM2->CONTROLS[0].CnV = compareZ;

}