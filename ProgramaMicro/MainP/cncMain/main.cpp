// #include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "UartCom.h"
#include "MklTime.h"


int main()
{
    LcdDisp Lcd;
    Uart Serial;
    Serial.init();
    Lcd.lcdInit();

    char receivedString[50];

    while (true) {
    Lcd.lcdClear();
    // Serial.sendString("Jalese compa\n");
    // Lcd.lcdPrint("Jalese compa");
    Serial.readString(receivedString, sizeof(receivedString), '\n');
    Lcd.lcdPrint(receivedString);
    Lcd.lcdPrint(receivedString);
    delay_ms(1000);
    }
}

