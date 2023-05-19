#ifndef KEYPAD_H
#define KEYPAD_H

#include <MKL25Z4.h>
#include "MklTime.h"
#include <stdint.h>

class KeyPad{

    int32_t D1 = (1<<1); //Row 1 
    int32_t D2 = (1<<2); //Row 2
    int32_t D3 = (1<<4); //Row 3
    int32_t D4 = (1<<12);  //Row 4
    int32_t D5 = (1<<4); //Column 1
    int32_t D6 = (1<<5); //Column 2
    int32_t D7 = (1<<8); //Column 3
    int32_t D8 = (1<<9); //Column 4

    
    char keypadValues[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    private:
        void rstRows();
        void clrRows();
        void setRows();
        void rstColumns();
        void setColumns();
    
    public:
        KeyPad();
        void init();
        char getKey();
};

#endif // KEYPAD_H