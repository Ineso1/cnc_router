#include <MklTime.h>
#include "TpmMod.h"
#include "Stepper.h"
#include "CNCController.h"

CNCController Control;
int main()
{
    Control.setPinX('C', 1, 'C', 2, 2000, 10, 0, 0, 1);
    Control.setPinY('B', 1, 'B', 0, 2000, 10, 1, 1, 2);
    Control.setPinZ('B', 3, 'B', 2, 2000, 10, 2, 1, 3);
    Control.moveTo(10, 20, 5);
    // Control.motorX.moveTo(10);
    // motor.moveTo(10);
    // __enable_irq();
    Control.motorX.enableTpm();
    while (true) {

    }
}

extern "C" void TPM0_IRQHandler() {

    if (TPM0->SC & TPM_SC_TOF_MASK)
    {
        TPM0->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag

        Control.motorX.addPulseCounter();

        if (Control.motorX.getPulseTarget() <= Control.motorX.getPulseCounter()) {
                Control.motorX.substractPulseCounter();
                Control.motorX.setChValue(0);
        }
        else {
        PTB->PTOR = 0x80000; /* toggle green LED */
            // delay_ms(1000);
        }
    }
}

extern "C" void TPM1_IRQHandler() {

    if (TPM1->SC & TPM_SC_TOF_MASK)
    {
        TPM1->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag

        Control.motorY.addPulseCounter();

        if (Control.motorY.getPulseTarget() <= Control.motorY.getPulseCounter()) {
            Control.motorY.substractPulseCounter();
            Control.motorY.setChValue(0);
        }
        else {
        PTB->PTOR = 0x40000; /* toggle red LED */
            // delay_ms(1000);
        }
    }
}

extern "C" void TPM2_IRQHandler() {
    
    if (TPM2->SC & TPM_SC_TOF_MASK)
    {
        TPM2->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag

        Control.motorZ.addPulseCounter();

        if (Control.motorZ.getPulseTarget() <= Control.motorZ.getPulseCounter()) {
            Control.motorZ.substractPulseCounter();
            Control.motorZ.setChValue(0);
        }
        else {
        PTD->PTOR = 0x02; /* toggle blue LED */
            // delay_ms(1000);
        }
    }
}