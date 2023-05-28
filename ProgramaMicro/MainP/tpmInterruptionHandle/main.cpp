#include <MKL25Z4.h>
#include "MklTime.h"

int pulses = 0;
int target = 10;

void tpm0Config(){
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // Enable TPM0 clock
    NVIC_SetPriority(TPM0_IRQn, 2);       // priority of TPM0 interruption
    NVIC_EnableIRQ(TPM0_IRQn);  
    TPM0->SC = 0;
    TPM0->SC = 0x07; /* prescaler /128 */
    TPM0->MOD = 0xFFFF; /* max modulo value */
    TPM0->SC |= 0x80; /* clear TOF */
    TPM0->SC |= 0x40; /* enable timeout interrupt */
    TPM0->SC |= 0x08; /* enable timer */
}

void tpm1Config(){
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // Enable TPM0 clock
    NVIC_SetPriority(TPM1_IRQn, 2);       // priority of TPM0 interruption
    NVIC_EnableIRQ(TPM1_IRQn);
    TPM1->SC = 0;
    TPM1->SC = 0x07; /* prescaler /128 */
    TPM1->MOD = 0xFFFF; /* max modulo value */
    TPM1->SC |= 0x80; /* clear TOF */
    TPM1->SC |= 0x40; /* enable timeout interrupt */
    TPM1->SC |= 0x08; /* enable timer */
}

void tpm2Config(){
    SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;  // Enable TPM0 clock
    NVIC_SetPriority(TPM2_IRQn, 2);       // priority of TPM0 interruption
    NVIC_EnableIRQ(TPM2_IRQn);  
    TPM2->SC = 0;
    TPM2->SC = 0x07; /* prescaler /128 */
    TPM2->MOD = 0xFFFF; /* max modulo value */
    TPM2->SC |= 0x80; /* clear TOF */
    TPM2->SC |= 0x40; /* enable timeout interrupt */
    TPM2->SC |= 0x08; /* enable timer */
}



void RgbToogleBlue(){
    PTD->PTOR = 0x02; /* toggle blue LED */
}

void RgbToogleRed(){
    PTB->PTOR = 0x40000; /* toggle red LED */
}

void RgbToogleGreen(){
    PTB->PTOR = 0x80000; /* toggle green LED */
}

void RgbOff(){
    PTD->PDOR |= 0x02; /* off blue LED */
    PTB->PDOR |= 0x40000; /* toggle red LED */
    PTB->PDOR |= 0x80000; /* toggle green LED */
}

void RgbConfig(){
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

void init(){
    __disable_irq();
    RgbConfig();
    SIM->SOPT2 |= 0x01000000; 
    tpm0Config();
    tpm1Config();

}


int main(){
    init();
    RgbOff();
    __enable_irq();
    while (true) {
        // RgbOff();
        // PTD->PDOR = 0x02; /* toggle blue LED */
        // delay_ms(2500);
    }
}

extern "C" void TPM0_IRQHandler(void) {
pulses ++;
if (pulses >= target) {
    PTB->PDOR &= ~0x40000; /* toggle red LED */
}
else {
    PTB->PTOR = 0x80000; /* toggle green LED */
    delay_ms(1000);
}
// PTB->PDOR &= ~0x40000; /* toggle red LED */
// PTB->PDOR |= 0x40000; /* toggle red LED */
TPM0->SC |= 0x80; /* clear TOF */

}

extern "C" void TPM1_IRQHandler(void) {
// PTB->PDOR &= ~0x80000; /* toggle green LED */
// PTB->PDOR |= 0x80000; /* toggle green LED */
TPM1->SC |= 0x80; /* clear TOF */
}