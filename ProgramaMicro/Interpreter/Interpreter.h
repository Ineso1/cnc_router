#ifndef MOVEMOTOR_H
#define MOVEMOTOR_H

#include "Stepper.h"
#include <math.h>
#include <stdio.h>

// Define the pins for each axis
const int X_STEP_PIN = 2;
const int X_ENABLE_PIN = 3;
const int X_DIR_PIN = 4;

const int Y_STEP_PIN = 5;
const int Y_ENABLE_PIN = 6;
const int Y_DIR_PIN = 7;

const int Z_STEP_PIN = 8;
const int Z_ENABLE_PIN = 9;
const int Z_DIR_PIN = 10;

// Create instances of the stepper motor objects for each axis
Stepper xStepper;
Stepper yStepper;
Stepper zStepper;

// Initialize the CNC machine
void setup();

// Process a G-code line and move the CNC machine accordingly
void processGcodeLine(const char*, int*&, int*&, int*&);


#endif