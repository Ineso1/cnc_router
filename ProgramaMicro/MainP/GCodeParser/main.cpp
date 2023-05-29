#include <MKL25Z4.h>
#include "UartCom.h"
#include "LcdDisp.h"
#include "GCodeParser.h"


Uart Serial;
LcdDisp Lcd;
GCodeParser Parser;



// main() runs in its own thread in the OS
int main()
{
    Lcd.lcdInit();
    std::string gcode = "G1 X1234.12345 Y987.98 Z12.6789";
    double Xcode, Ycode, Zcode, Icode, Jcode, Rcode;
    Parser.extractGCodeComponents(gcode, Xcode, Ycode, Zcode, Icode, Jcode, Rcode);    
    Lcd.lcdClear();

    char buffer[50];
    Parser.doubleToCharArray( Xcode, buffer);

    Lcd.lcdPrint(buffer);
    while (true) {

    }
}

