#include <MKL25Z4.h>
#include "CNCController.h"
#include "Stepper.h"

// Stepper motorX;

// int main() {
//     motorX.setPins('C', 1, 'C', 2, 2000, 10, 0, 0);  
//     motorX.move_mm(1);
//     int distance = 10;

//     motorX.useTpm();
//     motorX.setChValue(10);
//     motorX.setTpmMod(13003);
//     motorX.setTpmPrescaler(32);

    
//     int directionX = (distance >= 0) ? 1 : 0;
//     motorX.setDirection(directionX);
    
//     motorX.enableTpm();

//     int dutyCycle = 0;
//     int step = 20;
    
//     while (true) {
//         motorX.setChValue(dutyCycle);

//         // Increase the duty cycle
//         dutyCycle += step;

//         if (dutyCycle >= 13003) {
//             dutyCycle = 13003;  // Limit to the maximum value
//             step = -20;  // Start decreasing the duty cycle for tpm1
//         } else if (dutyCycle <= 0) {
//             dutyCycle = 0;  // Limit to 0
//             step = 20;  // Start increasing the duty cycle for tpm1 again
//         }
//         // Delay for a short period
//         for (int i = 0; i < 50000; ++i) {
//             __NOP();
//         }
//     }
    
// }

CNCController Control;

int main(){
    Control.setPinX('C', 1, 'C', 2, 2000, 10, 0, 0, 1);
    Control.setPinY('B', 1, 'B', 0, 2000, 10, 1, 1, 2);
    Control.setPinZ('B', 3, 'B', 2, 2000, 10, 0, 0, 3);

    int dutyCycle = 0;
    int step = 20;
    // Control.moveZ(1);
    // Control.moveY(1);
    // Control.moveX(1);

    delay_ms(1000);
    Control.moveTo(1, 1, 1);

    while (true) {
        
    }
}