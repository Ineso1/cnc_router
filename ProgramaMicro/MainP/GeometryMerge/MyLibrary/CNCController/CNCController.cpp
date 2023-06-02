#include "CNCController.h"
#include <cstdint>
#include <string>
#include "Geometry.h"

CNCController::CNCController() {
    // Initialize position variables
    motorXPosition = 0;
    motorYPosition = 0;
    motorZPosition = 0;
    Serial.init();
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


int CNCController::maxStep(int a, int b, int c) {
  if (a >= b && a >= c) {
    return a;
  } else if (b >= a && b >= c) {
    return b;
  } else {
    return c;
  }
}

void CNCController::calculateModuleAndChannelValues(int prescaler, float pulseDuration, float minDelayBetween, int maxSteps, float& moduleValue, uint16_t& channelValue) {
    
    // Calculate the pulse frequency
    double timeInstruction = maxSteps * (pulseDuration + minDelayBetween);  // Convert pulse duration to seconds

    // Calculate the module value
    moduleValue =  ((timeInstruction *  clk_frequen) / prescaler);

    // Calculate the channel value
    channelValue = ((clk_frequen * pulseDuration ) / prescaler);
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

void CNCController::moveTo(float x, float y, float z){
    __disable_irq();

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
    float moduleValue;
    uint16_t channelValue;

    channelValue = ((clk_frequen * pulseDuration ) / prescaler);
    // Calculate the module and compare values based on the maximum steps
    calculateModuleAndChannelValues( prescaler, pulseDuration, minDelayBetween, maxSteps, moduleValue, channelValue);

    uint32_t XmoduleValue, YmoduleValue, ZmoduleValue;
    XmoduleValue = std::round(moduleValue * maxSteps / (stepsX));
    YmoduleValue = std::round(moduleValue * maxSteps / (stepsY));
    ZmoduleValue = std::round(moduleValue * maxSteps / (stepsZ));

    int mod = 1 * moduleValue;


    motorX.moveTo(x, XmoduleValue, channelValue, stepsX);
    motorY.moveTo(y, YmoduleValue, channelValue, stepsY);
    motorZ.moveTo(z, ZmoduleValue, channelValue, stepsZ);

    TPM0->MOD = XmoduleValue;
    TPM1->MOD = YmoduleValue;
    TPM2->MOD = ZmoduleValue;


    // Set the module and compare values for each TPM (Timer/Counter)

    __enable_irq();
    // Enable TPMs
    motorX.enableTpm();
    motorY.enableTpm();
    motorZ.enableTpm();

    while (!(motorX.tpmProcess() & motorY.tpmProcess() & motorZ.tpmProcess())) {}

}

void CNCController::moveArc(const Point& finalPoint, double radius, double spacing) {
    // Create an arc using the provided parameters
    Point currentPos(motorX.getPosition(), motorY.getPosition(), motorZ.getPosition());
    Arc arc( currentPos, finalPoint, radius);

    // Slice the arc into points
    std::vector<Point> arcPoints = arc.sliceArc(spacing);

    // Move to each point in the arc
    for (const Point& point : arcPoints) {
        moveTo(point.x(), point.y(), point.z());
    }
}

void CNCController::moveArc(const Point& finalPoint, const Point& center,  double spacing) {
    // Create an arc using the provided parameters
    Point currentPos(motorX.getPosition(), motorY.getPosition(), motorZ.getPosition());
    Arc arc(currentPos, finalPoint, center);

    // Slice the arc into points
    std::vector<Point> arcPoints = arc.sliceArc(spacing);

    // Move to each point in the arc
    for (const Point& point : arcPoints) {
        moveTo(point.x(), point.y(), point.z());
    }
}

void CNCController::moveDiagonal(const Point& end) {
    Point currentPos(motorX.getPosition(), motorY.getPosition(), motorZ.getPosition());

    // Calculate the distance between the start and end points
    double distanceX = end.x() - currentPos.x();
    double distanceY = end.y() - currentPos.y();
    double distanceZ = end.z() - currentPos.z();

    // Determine the maximum distance among the axes
    double maxDistance = std::abs(distanceX);
    if (std::abs(distanceY) > maxDistance)
        maxDistance = std::abs(distanceY);
    if (std::abs(distanceZ) > maxDistance)
        maxDistance = std::abs(distanceZ);

    // Calculate the step sizes for each axis
    double stepX = distanceX / maxDistance;
    double stepY = distanceY / maxDistance;
    double stepZ = distanceZ / maxDistance;

    // Move the CNC in steps along the diagonal line
    for (double i = 0; i <= maxDistance; i++) {
        double x = currentPos.x() + stepX * i;
        double y = currentPos.y() + stepY * i;
        double z = currentPos.z() + stepZ * i;
        moveTo(x, y, z);
    }
}

/*---------------------------------------------------------------------------Pendientes---------------------------------------------------------------------------*/

/**
 * @brief Initialize the stop switches for auto homing.
 */
void CNCController::initStopSwitches() {
    // Function implementation
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