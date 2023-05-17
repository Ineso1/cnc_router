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
    char receivedString[50];
    Lcd.lcdClear();

    while (true) {
        Serial.readString(receivedString, sizeof(receivedString), '\n');
        if (receivedString[0] != ' ') {
            Lcd.lcdClear();
            char* gcode = new char[strlen(receivedString) + 1];
            strcpy(gcode, receivedString);
            Lcd.lcdPrint(gcode);
            // gcodeQueue.push(gcode);
        }
    }
}

