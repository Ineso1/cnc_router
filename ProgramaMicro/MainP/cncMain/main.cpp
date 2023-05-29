// #include "mbed.h"
#include <MKL25Z4.h>
#include "LcdDisp.h"
#include "UartCom.h"
#include "MklTime.h"
#include "KeyPad.h"
#include "CNCController.h"
#include "GCodeParser.h"
#include <string>
#include <cstring>
#include <cstdlib>

LcdDisp Lcd;
Uart Serial;
KeyPad Pad;
CNCController Control;
GCodeParser Parser;

enum State {
    STATE_MENU,
    STATE_HOME,
    STATE_CONFIG,
    STATE_START
};


void clearRgb(){
    PTD->PDOR &= ~0x02; /* clr blue LED */
    PTB->PDOR &= ~0x40000; /* clr red LED */
    PTB->PDOR &= ~0x80000; /* clr green LED */
}


void setup(){
    Lcd.lcdInit();
    Serial.init();
    Pad.init();
    Control.setPinX('C', 1, 'C', 2, 2000, 10, 0, 0, 1);
    Control.setPinY('B', 1, 'B', 0, 2000, 10, 1, 1, 2);
    Control.setPinZ('B', 3, 'B', 2, 2000, 10, 2, 1, 3);
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

    delay_ms(2000);
    Control.moveX(1);
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
    
    while (!Pad.getKey() && !Serial.available()) {}
    delay_ms(2);
}

void ProcessGCodeLine(const std::string& gcodeLine) {
    Lcd.lcdClear();
    // Serial.sendString(gcodeLine);
    Lcd.lcdPrint(gcodeLine);
    double Xcode, Ycode, Zcode, Icode, Jcode, Rcode;
    std::string gcodeNumber;
    bool g;
    delay_ms(2000);
    Lcd.lcdClear();
    Parser.extractGCodeComponents(gcodeLine, g, gcodeNumber, Xcode, Ycode, Zcode, Icode, Jcode, Rcode); 
    if (gcodeLine == "0") {
        Lcd.lcdPrint("G0");
    } else if (gcodeLine == "1") {
        Lcd.lcdPrint("G1");
    }else if (gcodeLine == "2") {
        Lcd.lcdPrint("G2");
    } else if (gcodeLine == "3") {
        Lcd.lcdPrint("G3");
    }
    else {
        Lcd.lcdPrint("?");
    }  
    delay_ms(5000);
}

void Start(){
    Lcd.lcdClear();
    Lcd.lcdPrint("Start");
    delay_ms(200);
    Lcd.lcdClear();

    char gcodeLine[50];
    bool readyForNextLine = true;
    bool processFinished = false;

    while (!processFinished) {
        if (readyForNextLine) {
            memset(gcodeLine, 0, sizeof(gcodeLine)); // Clean gcodeLine
            Lcd.lcdPrint("waiting for line");
            while (!Serial.available()) {}
            Serial.readLine(gcodeLine, sizeof(gcodeLine));
            Lcd.lcdClear();
            readyForNextLine = false;
            
            if (Serial.receivedFinishFlag(gcodeLine)) {
                processFinished = true;
                readyForNextLine = true;
            }
            else {
                ProcessGCodeLine(gcodeLine);
                readyForNextLine = true;
            }
            if (readyForNextLine && !processFinished) {
                Serial.sendString("ready");
            }
        }
    }
    Lcd.lcdClear();
    Lcd.lcdPrint("Finalizado");
    Serial.sendString("Finalizado");
    delay_ms(2000);
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

                case '3':
                    mainState = STATE_START;
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

                case '3':
                    mainState = STATE_START;
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
            case STATE_START:
                Start();
                mainState = STATE_MENU;
                op_s = ' ';
                op_c = ' ';
            break;
            default:
                Menu();
            break;
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