#include <MKL25Z4.h>
#include <stdint.h>
#include "KeyPad.h"
#include "LcdDisp.h"

KeyPad Pad;
LcdDisp Lcd;
Uart SerialShit;

// main() runs in its own thread in the OS
int main()
{
    SerialShit.init();
    Lcd.lcdInit();
    Pad.init();

    char key;

    while (true) {
        key = Pad.getKey();
        if(key){
            Lcd.lcdClear();
            SerialShit.sendChar(key);
            Lcd.lcdData(key);
        }

    }
}
