// #include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "UartCom.h"
#include "MklTime.h"
#include <queue>

std::queue<char*> gcodeQueue;  // Queue to store G-code instructions
LcdDisp Lcd;
Uart Serial;

void processGcode(const char* gcode) {
    // Process the G-code instruction here
    Lcd.lcdClear();
    Lcd.lcdPrint(gcode);
}

int main()
{
    Serial.init();
    Lcd.lcdInit();
    const int bufferSize = 100;
    char receivedString[bufferSize];
    Lcd.lcdClear();

    while (true) {
        if (Serial.available()) {
            Serial.readString(receivedString, bufferSize, &Lcd);
            // Process the received string as needed
            Lcd.lcdClear();
            Lcd.lcdPrint(receivedString);
        }
    }
}
