// #include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "UartCom.h"
#include "MklTime.h"
#include "KeyPad.h"
#include <queue>

std::queue<char*> gcodeQueue;  // Queue to store G-code instructions
LcdDisp Lcd;
// Uart Serial;
KeyPad Pad;

void processGcode(const char* gcode) {
    // Process the G-code instruction here
    Lcd.lcdClear();
    Lcd.lcdPrint(gcode);
}

int main()
{
    // Serial.init();
    Lcd.lcdInit();
    Pad.init();
    const int bufferSize = 100;
    char receivedString[bufferSize];
    // Lcd.lcdClear();
    char key = ' ';

    while (true) {
        key = Pad.getKey();
        // if(key != nullptr){
        //     // Serial.sendString("si");
        // }
        // Lcd.lcdPrint(key);
        delay_ms(100);
        // if (Serial.available()) {
        //     Serial.readString(receivedString, bufferSize, &Lcd);
        //     // Process the received string as needed
        //     Lcd.lcdClear();
        //     Lcd.lcdPrint(receivedString);
        // }
    }
}
