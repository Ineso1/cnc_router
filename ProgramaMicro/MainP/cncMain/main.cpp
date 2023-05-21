// #include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "UartCom.h"
#include "MklTime.h"
#include "KeyPad.h"
#include <string>
#include <cstring>
#include <cstdlib>

LcdDisp Lcd;
Uart Serial;
KeyPad Pad;

enum State {
    STATE_MENU,
    STATE_HOME,
    STATE_CONFIG
};

void setup(){
    Lcd.lcdInit();
    Serial.init();
    Pad.init();

    Lcd.lcdPrint("Iniciando...");
    delay_ms(5000);
    Lcd.lcdClear();
}


void Home(float xCoord, float yCoord, float zCoord, const char* coordType) {
    
    Lcd.lcdClear();
    Lcd.lcdPrint("Home");
    delay_ms(2000);
    Lcd.lcdClear();

    std::string dataString = "X: " + std::to_string((int)xCoord) + " Y: " + std::to_string((int)yCoord) + " Z: " + std::to_string((int)zCoord);
    
    Lcd.lcdPrint(dataString);
    Lcd.lcdSetCursor(1, 0);
    Lcd.lcdPrint(coordType);
    
    while (!Pad.getKey() && !Serial.available()) { }
    delay_ms(2);
}

void Config(){
    Lcd.lcdClear();
    Lcd.lcdPrint("Configuration");
    while (!Pad.getKey() && !Serial.available()){}
    delay_ms(2);
}

void Menu(){
    
    Lcd.lcdClear();
    Lcd.lcdPrint("Menu");
    delay_ms(2000);
    Lcd.lcdClear();

    std::string dataString1 = "1:Home 2:Config ";
    std::string dataString2 = "3:Start 4:Other ";
    
    Lcd.lcdPrint(dataString1);
    Lcd.lcdSetCursor(1, 0);
    Lcd.lcdPrint(dataString2);
    
    while (!Pad.getKey() && !Serial.available()) { }
    delay_ms(2);
}

int main()
{
    setup();
    float xCoord = 0, yCoord = 0, zCoord = 0;
    char coordType[] = "absolute";
    char op_c = ' ';
    char op_s = ' ';
    State mainState = STATE_MENU;

    while (true) {
        op_c = Pad.getKey();
        if (Serial.available()) {
            op_s = Serial.readChar();
        }

        if (op_s != ' ') {
            switch(op_s){
                case '0':
                    mainState = STATE_MENU;
                break;

                case '1':
                    mainState = STATE_HOME;
                break;

                case '2':
                    mainState = STATE_CONFIG;
                break;

                default:
                    mainState = STATE_MENU;
                break;
            }
        }

        if (op_c) {
            switch(op_c){
                case '0':
                    mainState = STATE_MENU;
                break;

                case '1':
                    mainState = STATE_HOME;
                break;

                case '2':
                    mainState = STATE_CONFIG;
                break;

                default:
                    mainState = STATE_MENU;
                break;
            }
        }

        switch (mainState) {
            case STATE_MENU:
                Menu();
            break;
            case STATE_HOME:
                Home(xCoord, yCoord, zCoord, coordType);
            break;
            case STATE_CONFIG:
                Config();
            break;
            default:
                Menu();
            break;
        }

    }
}
