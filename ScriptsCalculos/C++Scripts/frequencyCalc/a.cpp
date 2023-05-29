// #include <iostream>
// #include <cmath>

// int max(int a, int b, int c) {
//   if (a >= b && a >= c) {
//     return a;
//   } else if (b >= a && b >= c) {
//     return b;
//   } else {
//     return c;
//   }
// }

// void calculateModuleAndChannelValues(int clockFrequency, int prescaler, float pulseDuration, float minDelayBetween, int maxSteps, uint32_t& moduleValue, uint32_t& channelValue) {
//     // Calculate the maximum number of steps among all motors
    
//     // Calculate the pulse frequency
//     std::cout << "pulseDuration: " << pulseDuration << std::endl;
//     std::cout << "pulseDelay: " << minDelayBetween << std::endl;

//     float timeInstruction = maxSteps * (pulseDuration + minDelayBetween);  // Convert pulse duration to seconds
//     std::cout << "Instruction: " << timeInstruction << std::endl;

//     // Calculate the module value
//     moduleValue = static_cast<uint32_t>(std::round( timeInstruction * (clockFrequency / prescaler)));

//     // Calculate the channel value
//     channelValue = ((clockFrequency * pulseDuration ) / prescaler);
// }

// int main() {
//     int clockFrequency = 42000000;  // 42 MHz
//     int prescaler = 128;
//     float pulseDuration = 0.0002;  // in seconds
//     float minDelayBetween = 0.0003;
//     int stepsX = 1;  // Number of steps for motor X
//     int stepsY = 1;  // Number of steps for motor Y
//     int stepsZ = 1;  // Number of steps for motor Z

//     int maxSteps = max(stepsX, stepsY, stepsZ);
//     std::cout << "Max: " << maxSteps << std::endl;


//     uint32_t moduleValue, channelValue;
//     calculateModuleAndChannelValues(clockFrequency, prescaler, pulseDuration, minDelayBetween, maxSteps, moduleValue, channelValue);

//     std::cout << "Module Value: " << moduleValue << std::endl;
//     std::cout << "Channel Value: " << channelValue << std::endl;

//     uint32_t XmoduleValue, YmoduleValue, ZmoduleValue;
//     XmoduleValue = moduleValue * (maxSteps/stepsX);
//     YmoduleValue = moduleValue * (maxSteps/stepsY); 
//     ZmoduleValue = moduleValue * (maxSteps/stepsZ); 

//     std::cout << "Module Value X: " << XmoduleValue << std::endl;
//     std::cout << "Module Value Y: " << YmoduleValue << std::endl;
//     std::cout << "Module Value Z: " << ZmoduleValue << std::endl;


//     return 0;
// }






#include <iostream>
#include <cmath>
const float PI = 3.1415926535897;


int max(int a, int b, int c) {
  if (a >= b && a >= c) {
    return a;
  } else if (b >= a && b >= c) {
    return b;
  } else {
    return c;
  }
}

int calculateSteps_mm(float target, float position, int motorSteps, int radius) {
    int distance = target - position;
    if (distance < 0) {
        distance *= -1;
        std::cout << true ;
    }
    else {
        std::cout << false ;
    }
    return ((distance * motorSteps) / (2.0 * PI * radius));
}

void calculateModuleAndChannelValues(int clockFrequency, int prescaler, float pulseDuration, float minDelayBetween, int maxSteps, uint32_t& moduleValue, uint32_t& channelValue) {
    // Calculate the pulse frequency
    std::cout << "pulseDuration: " << pulseDuration << std::endl;
    std::cout << "pulseDelay: " << minDelayBetween << std::endl;

    float timeInstruction = maxSteps * (pulseDuration + minDelayBetween);  // Convert pulse duration to seconds
    std::cout << "Instruction: " << timeInstruction << std::endl;

    // Calculate the module value
    moduleValue = static_cast<uint32_t>(std::round((timeInstruction * clockFrequency) / prescaler));

    // Calculate the channel value
    channelValue = ((clockFrequency * pulseDuration ) / prescaler);
}

int main() {
    int clockFrequency = 42000000;  // 42 MHz
    int prescaler = 128;
    float pulseDuration = 0.0002;  // in seconds
    float minDelayBetween = 0.0003;
    int motorSteps = 200;
    float radius;


    float positionX = 0;
    float positionY = 0;
    float positionZ = 0;


    float targetX = 10;
    float targetY = 2;
    float targetZ = 5;


    int stepsX = 5;  // Number of steps for motor X
    int stepsY = 1;  // Number of steps for motor Y
    int stepsZ = 20;  // Number of steps for motor Z


    stepsX = calculateSteps_mm(targetX, positionX, motorSteps, radius);
    stepsY = calculateSteps_mm(targetY, positionY, motorSteps, radius);
    stepsZ = calculateSteps_mm(targetZ, positionZ, motorSteps, radius);


    int maxSteps = max(stepsX, stepsY, stepsZ);
    std::cout << "Max: " << maxSteps << std::endl;

    uint32_t moduleValue, channelValue;
    calculateModuleAndChannelValues(clockFrequency, prescaler, pulseDuration, minDelayBetween, maxSteps, moduleValue, channelValue);

    std::cout << "Module Value: " << moduleValue << std::endl;
    std::cout << "Channel Value: " << channelValue << std::endl;

    uint32_t XmoduleValue, YmoduleValue, ZmoduleValue;
    XmoduleValue = moduleValue * ( maxSteps / static_cast<float>(stepsX));
    YmoduleValue = moduleValue * ( maxSteps / static_cast<float>(stepsY));
    ZmoduleValue = moduleValue * ( maxSteps / static_cast<float>(stepsZ));

    std::cout << "Module Value X: " << XmoduleValue << std::endl;
    std::cout << "Module Value Y: " << YmoduleValue << std::endl;
    std::cout << "Module Value Z: " << ZmoduleValue << std::endl;

    return 0;
}
