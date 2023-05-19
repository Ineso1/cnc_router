#include "KeyPad.h"

/**
 * @file KeyPad.cpp
 *
 * @brief Class for numeric matrix keypad initialization.
 */


KeyPad::KeyPad(){}

void KeyPad::rstRows(){
    PTA->PDDR &= ~ ( D1 | D2 | D4 );
    PTD->PDDR &= ~ ( D3 ); 
}

void KeyPad::clrRows(){
    PTA->PCOR = ( D1 | D2 | D4 );
    PTD->PCOR = ( D3 ); 
}

void KeyPad::setRows(){
    PTA->PDDR |= ( D1 | D2 | D4 );
    PTD->PDDR |= ( D3 ); 
}

void KeyPad::rstColumns(){
    PTA->PDDR &= ~( D5 | D6 );
    PTC->PDDR &= ~( D7 | D8 ); 
}

void KeyPad::setColumns(){
    PTA->PDDR |= ( D5 | D6 );
    PTC->PDDR |= ( D7 | D8 ); 
}

void KeyPad::init(){

    SIM->SCGC5 |= ( SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK );
    PORTA->PCR[1] = 0x103; //Row (PTA1) D1
    PORTA->PCR[2] = 0x103; //Row (PTA2) D2
    PORTD->PCR[4] = 0x103; //Row (PTD4) D3
    PORTA->PCR[12] = 0x103; //Row (PTA12) D4
    PORTA->PCR[4] = 0x103; //Column (PTA4) D5
    PORTA->PCR[5] = 0x103; //Column (PTA5) D6
    PORTC->PCR[8] = 0x103; //Column (PTC8) D7
    PORTC->PCR[9] = 0x103; //Column (PTC9) D8

    //Make all rows as input
    setRows();

    //Make all columns output
    rstColumns();

}

char KeyPad::getKey(){
    SerialShit.sendString("getting shit");
    delay_ms(100);
    int row, col;

    /* one row is active */
    /* check to see any key pressed */

    setRows(); /* enable all rows */
    clrRows();

    delay_us(2); /* wait for signal return */

    col = (PTA->PDIR & (D5 | D6)) | ((PTC->PDIR & (D7 | D8))>>2); /* read all columns */

    char BinaryBuffer[33];
    // UART0_IntToBinary(PTC->PDOR, pdorBinaryBuffer, sizeof(pdorBinaryBuffer));
    // UART0_SendString("PTC_PDOR: ");
    // UART0_SendString(pdorBinaryBuffer);
    // UART0_SendString("\r\n");
    SerialShit.intToBinary(col, BinaryBuffer, sizeof(BinaryBuffer));
    SerialShit.sendString(BinaryBuffer);
    SerialShit.sendString("\r\n");

    rstRows();
    rstColumns();

    if (col == 0xF0)
    {
        return 0; /* No pressed */
    }

    int32_t rows[] = {D1,D2,D3,D4};
    /* If a key is pressed, we need find out which key.*/ 
    for (row = 0; row < 4; row++)
    {   
        rstRows();
        rstColumns();

        if(row <= 1 || row == 12){
            PTA->PDDR |= rows[row];
            PTA->PCOR = rows[row];
        }
        else{
            PTD->PDDR |= rows[row];
            PTD->PCOR = rows[row];
        }

        delay_us(2); /* wait for signal to settle */
        col = (PTA->PDIR & (D5 | D6)) | ((PTC->PDIR & (D7 | D8))>>2); /* read all columns */

        if (col != 0xF0) break; 
        /* if one of the input is low, some key is pressed. */
    }
    rstRows();
    rstColumns();

    if (row == 4)
    return 0; /* if we get here, no key is pressed */

    if (col == 0xE0) return row * 4 + 1; /* key in column 0 */
    if (col == 0xD0) return row * 4 + 2; /* key in column 1 */
    if (col == 0xB0) return row * 4 + 3; /* key in column 2 */
    if (col == 0x70) return row * 4 + 4; /* key in column 3 */
    return 0; /* just to be safe */
}