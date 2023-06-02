#include <MKL25Z4.h>
#include <stdint.h>
#include "KeyPad.h"
#include "LcdDisp.h"
#include "CNCController.h"
#include "Point.h"

KeyPad Pad;
LcdDisp Lcd;
Uart SerialShit;
CNCController Control;


void init(){
    SerialShit.init();
    Lcd.lcdInit();
    Pad.init();
    Control.setPinX('C', 1, 'C', 2, 200, 10, 0, 0, 1);
    Control.setPinY('B', 1, 'B', 0, 200, 10, 1, 1, 2);
    Control.setPinZ('B', 3, 'B', 2, 200, 10, 2, 1, 3);
}

// main() runs in its own thread in the OS
int main()
{
    init();

    char key;

    while (true) {
        key = Pad.getKey();
        while (!Pad.getKey()){}
        if(key == '0'){
            Lcd.lcdClear();
            SerialShit.sendChar(key);
            Lcd.lcdData(key);
            Point pFinal(12,10,0);
            Control.moveArc(pFinal, 10, 1);
        }
        if(key == '1'){
            Lcd.lcdClear();
            SerialShit.sendChar(key);
            Lcd.lcdData(key);
            Control.moveX(-1);
        }
        if(key == '2'){
            Lcd.lcdClear();
            SerialShit.sendChar(key);
            Lcd.lcdData(key);
            Control.moveTo(1,1,1);
        }
        else {
            Lcd.lcdClear();
            SerialShit.sendChar(key);
            Lcd.lcdData(key);
        }

    }
}


extern "C" void TPM0_IRQHandler() {

    if (TPM0->SC & TPM_SC_TOF_MASK)
    {
        TPM0->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag

        Control.motorX.addPulseCounter();

        if (Control.motorX.getPulseTarget() <= Control.motorX.getPulseCounter() - 2) {
                Control.motorX.substractPulseCounter();
                Control.motorX.setChValue(0);
        }
        else {
        SerialShit.sendString("X " + std::to_string(Control.motorX.getPulseCounter()) + "\n");
        }
    }
}

extern "C" void TPM1_IRQHandler() {

    if (TPM1->SC & TPM_SC_TOF_MASK)
    {
        TPM1->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag

        Control.motorY.addPulseCounter();

        if (Control.motorY.getPulseTarget() <= Control.motorY.getPulseCounter() - 2) {
            Control.motorY.substractPulseCounter();
            Control.motorY.setChValue(0);
        }
        else {
        SerialShit.sendString("Y " + std::to_string(Control.motorY.getPulseCounter()) + "\n");
        }
    }
}

extern "C" void TPM2_IRQHandler() {
    
    if (TPM2->SC & TPM_SC_TOF_MASK)
    {
        TPM2->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag

        Control.motorZ.addPulseCounter();


        if (Control.motorZ.getPulseTarget() <= Control.motorZ.getPulseCounter() - 2) {
            Control.motorZ.substractPulseCounter();
            Control.motorZ.setChValue(0);
        }
        else {
        SerialShit.sendString("Z " + std::to_string(Control.motorZ.getPulseCounter()) + "\n");
        }
    }
}