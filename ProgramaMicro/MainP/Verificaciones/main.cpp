#include <MKL25Z4.h>
#include <stdint.h>
#include "KeyPad.h"
#include "LcdDisp.h"
#include "Stepper.h"
#include "CNCController.h"

KeyPad Pad;
LcdDisp Lcd;
Uart SerialShit;
CNCController Control;
Stepper motorX;

// main() runs in its own thread in the OS
int main()
{
    SerialShit.init();
    Lcd.lcdInit();
    Pad.init();
    Control.setPinX('C', 1, 'C', 2, 2000, 10, 2);
    Control.setPinY('B', 3, 'B', 2, 2000, 10, 3);
    Control.setPinZ('B', 1, 'B', 0, 2000, 10, 4);

    char key;

    while (true) {
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
