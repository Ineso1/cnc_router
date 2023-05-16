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

    while (true) {
        Lcd.lcdClear();
        // Serial.sendString("Jalese compa\n");
        // Lcd.lcdPrint("Jalese compa");
        if (Serial.available()) {
        Serial.readString(receivedString, sizeof(receivedString), '\n');
        if (receivedString[0] != ' ') {
            char* gcode = new char[strlen(receivedString) + 1];
            strcpy(gcode, receivedString);
            gcodeQueue.push(gcode);
        }
    }
    }
}

