#include <MKL25Z4.h>
#include <stdint.h>
#include "KeyPad.h"
#include "LcdDisp.h"
#include "CNCController.h"

KeyPad Pad;
LcdDisp Lcd;
Uart SerialShit;
CNCController Control;


void clearRgb(){
    PTD->PDOR &= ~0x02; /* clr blue LED */
    PTB->PDOR &= ~0x40000; /* clr red LED */
    PTB->PDOR &= ~0x80000; /* clr green LED */
}


void init(){
    SerialShit.init();
    Lcd.lcdInit();
    Pad.init();
    Control.setPinX('C', 1, 'C', 2, 2000, 10, 0, 0, 1);
    Control.setPinY('B', 1, 'B', 0, 2000, 10, 1, 1, 2);
    Control.setPinZ('B', 3, 'B', 2, 2000, 10, 2, 1, 3);
}

// main() runs in its own thread in the OS
int main()
{
    init();

    Control.moveTo(10, 20, 5);
    Control.motorX.enableTpm();

    char key;

    while (true) {
        clearRgb();
        key = Pad.getKey();
        while (!Pad.getKey()){}
        if(key == '0'){
            Lcd.lcdClear();
            SerialShit.sendChar(key);
            Lcd.lcdData(key);
            Control.moveX(1);
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