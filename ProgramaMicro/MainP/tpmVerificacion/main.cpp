#include <MKL25Z4.h>
#include "TpmMod.h"

TpmMod tpm1;
TpmMod tpm2;

int main() {
    tpm1.init(0, 0);
    tpm2.init(1, 1);

    tpm1.enablePort('C', 1);
    tpm2.enablePort('B', 1);

    tpm1.setChannelValue(10);
    tpm2.setChannelValue(10);

    tpm1.setModulo(43703);
    tpm2.setModulo(13003);

    tpm1.setPrescaler(16);
    tpm2.setPrescaler(32);

    tpm1.enable();
    tpm2.enable();

    int dutyCycle1 = 0;
    int dutyCycle2 = 0;
    int step1 = 10;
    int step2 = 20;

    while (1) {
        tpm1.setChannelValue(dutyCycle1);
        tpm2.setChannelValue(dutyCycle2);

        // Increase the duty cycle
        dutyCycle1 += step1;
        dutyCycle2 += step2;

        if (dutyCycle1 >= 43703) {
            dutyCycle1 = 43703;  // Limit to the maximum value
            step1 = -10;  // Start decreasing the duty cycle for tpm1
        } else if (dutyCycle1 <= 0) {
            dutyCycle1 = 0;  // Limit to 0
            step1 = 10;  // Start increasing the duty cycle for tpm1 again
        }

        if (dutyCycle2 >= 13003) {
            dutyCycle2 = 13003;  // Limit to the maximum value
            step2 = -20;  // Start decreasing the duty cycle for tpm2
        } else if (dutyCycle2 <= 0) {
            dutyCycle2 = 0;  // Limit to 0
            step2 = 20;  // Start increasing the duty cycle for tpm2 again
        }

        // Delay for a short period
        for (int i = 0; i < 50000; ++i) {
            __NOP();
        }
    }
}
