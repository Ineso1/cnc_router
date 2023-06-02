#include "RGBLed.h"

/**
 * @brief Toggles the blue LED on the RGB LED module.
 */
void RGBLed::RgbToogleBlue(){
    PTD->PTOR = 0x02; /* toggle blue LED */
}

/**
 * @brief Toggles the red LED on the RGB LED module.
 */
void RGBLed::RgbToogleRed(){
    PTB->PTOR = 0x40000; /* toggle red LED */
}

/**
 * @brief Toggles the green LED on the RGB LED module.
 */
void RGBLed::RgbToogleGreen(){
    PTB->PTOR = 0x80000; /* toggle green LED */
}

/**
 * @brief Turns off all the LEDs on the RGB LED module.
 */
void RGBLed::RgbOff(){
    PTD->PDOR |= 0x02; /* off blue LED */
    PTB->PDOR |= 0x40000; /* toggle red LED */
    PTB->PDOR |= 0x80000; /* toggle green LED */
}


/**
 * @brief Configures the necessary registers and pins for controlling the RGB LED.
 *        It enables clocks to Port B and Port D, sets certain pins as GPIO pins,
 *        and configures the data direction registers to set the necessary pins as output.
 *        It also turns off all the LEDs initially.
 */
void RGBLed::RgbConfig(){
    SIM->SCGC5 |= 0x400; /* enable clock to Port B */
    SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
    PORTB->PCR[18] = 0x100; /* make PTB18 pin as GPIO */
    PTB->PDDR |= 0x40000; /* make PTB18 as output pin */
    PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */
    PTB->PDDR |= 0x80000; /* make PTB19 as output pin */
    PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
    PTD->PDDR |= 0x02; /* make PTD1 as output pin */
    RgbOff();
}
