#include "Interpreter.h"


// Initialize the CNC machine
void setup() {
  // Set the pins for each axis
  xStepper.setPins(X_STEP_PIN, X_ENABLE_PIN, 200, 5);
  yStepper.setPins(Y_STEP_PIN, Y_ENABLE_PIN, 200, 5);
  zStepper.setPins(Z_STEP_PIN, Z_ENABLE_PIN, 200, 5);
}

// Process a G-code line and move the CNC machine accordingly
void processGcodeLine(const char* gcode, int*&xx, int*&yy, int*&zz) {
  // Parse the G-code line to extract the relevant information
  
  // Example: Assuming the G-code line is in the format "G2 X10 Y20 I5 J10" or "G3 X10 Y20 I5 J10"
  char command;
  float x = 0.0;
  float y = 0.0;
  float z = 0.0;
  float i = 0.0;
  float j = 0.0;
  sscanf(gcode, "G%c X%f Y%f I%f J%f", &command, &x, &y, &i, &j);
  
  // Move the CNC machine based on the G-code command and coordinates
  if (command == '1') {
    // Move in a linear path
    
    // Move the X-axis
    xStepper.move_mm(x, xx, yy, zz);
    
    // Move the Y-axis
    yStepper.move_mm(y, xx, yy, zz);
    
    // Move the Z-axis
    zStepper.move_mm(z, xx, yy, zz);
  }
  else if (command == '2' || command == '3') {
    // Move in a circular path (clockwise or counterclockwise)
    
    // Calculate the center coordinates
    float centerX = x + i;
    float centerY = y + j;
    
    // Calculate the radius and start angle
    float radius = sqrt(i * i + j * j);
    float startAngle = atan2(j, i);
    
    // Calculate the number of steps to move based on the circular path
    int stepsToMove = (int)(radius * steps * (command == '3' ? -1.0 : 1.0)) / (2.0 * PI * radius);
    
    // Move the CNC machine in a circular path
    for (int i = 0; i < stepsToMove; i++) {
      float angle = startAngle + (i * 2.0 * PI) / stepsToMove;
      float newX = centerX + radius * cos(angle);
      float newY = centerY + radius * sin(angle);
      
      // Move the X-axis
      xStepper.move_mm(newX, &x, &y, &z);
      
      // Move the Y-axis
      yStepper.move_mm(newY, &x, &y, &z);
      
      // Move the Z-axis
      zStepper.move_mm(z, &x, &y, &z);
    }
  }
  // Add more shit over here
}

/*Use example*/
// // Main program loop
// void loop() {
//   // Read the G-code line from the serial port 
//   char gcodeLine[50];
//   readGcodeLine(gcodeLine, sizeof(gcodeLine));
//   // Process the G-code line and move the CNC machine
//   processGcodeLine(gcodeLine);
// }


#endif