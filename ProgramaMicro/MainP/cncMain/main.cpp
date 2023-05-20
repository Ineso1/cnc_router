// #include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "UartCom.h"
#include "MklTime.h"
#include "KeyPad.h"
#include <queue>


void processGcode(const char* gcode) {
    // Process the G-code instruction here
    Lcd.lcdClear();
    Lcd.lcdPrint(gcode);
}

int main()
{

    while (true) {

    }
}
