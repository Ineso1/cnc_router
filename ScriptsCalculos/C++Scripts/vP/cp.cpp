/* Toggling blue LED using TPM0 delay (prescaler)
* This program uses TPM0 to generate maximal delay to
* toggle the blue LED.
* MCGFLLCLK (41.94 MHz) is used as timer counter clock.
* Prescaler is set to divided by 128 and the Modulo register
* is set to 65,535. The timer counter overflows at
* 41.94 MHz / 128 / 65,536 = 5.0 Hz
*
* The blue LED is connected to PTD1.
*/
#include <MKL25Z4.H>
#include <stdbool.h>

void delayMs(int n) {
    int i;
    SysTick->LOAD = 41940 - 1;
    SysTick->CTRL = 0x5; /* Enable the timer and choose sysclk as the clock source */

    for(i = 0; i < n; i++) {
        while((SysTick->CTRL & 0x10000) == 0)
        /* wait until the COUNT flag is set */
        { }
    }
    SysTick->CTRL = 0; 
    /* Stop the timer (Enable = 0) */
/* turn on or off the LEDs wrt to bit 2-0 of the value */
}

//Inicializacion de contador y limpieza de bandera de overflow
void enableTpm(){
    TPM0->SC = 0x02; /* prescaler /4 ayuda a extender la cuenta del contador  */
    TPM0->SC |= 0x80; /* clear TOF */
    TPM0->SC |= 0x08; /* enable timer free-running mode */
}

void disableTpm(){
    TPM0->SC = 0;
}



//Contador a 3s, detectando alguna accion en pooling
//  *  Boton en  periodo de 3s retorna Verdadero
//  *  Boton en  periodo de 3s retorna Falso

bool conteo(){

    enableTpm();
    //Esperar a q la bandera de overflow se active
    /*Probable q la logica de while este inveritda, habria que probar*/
    while((TPM0->SC & 0x80) == 0){

        //Si se detecta sennal de boton
        if (PTA->PDIR & 0x20000){
            //Prender Led puerto E pin 0, desabilitar contador y retorno verdadero
            PTE->PDOR &= ~(1<<0);
            disableTpm();
            return true;
        }
        //Si no se detecta sennal de boton
        else{
            //Apagar boton
            PTE->PDOR |= (1<<0);
        }

        //Toggle Led
        PTD->PTOR |= 0x02;
        delayMs(100);
    } // wait until the TOF is set
    
    //A la terminacion del conteo retorno falso
    TPM0->SC |= 0x80; // clear TOF
    return false;
}

int main (void) {
    int time = 24576;
 
    SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
    SIM->SCGC5 |= 0x200; /* enable clock to Port A */
    SIM->SCGC5 |= 0x2000; /* clock to PORT E */
    SIM->SCGC5 |= 0x400; /* enable clock to Port B */

    //Led azul
    PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
    PTD->PDDR |= 0x02; /* make PTD1 as output pin */
    PTD->PDOR |= 0x02; /*Off*/


    //Led verde
    PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */
    PTB->PDDR |= 0x80000; /* make PTB19 as output pin */
    PTB->PDOR |= 0x80000; /*Off*/


    //Timer
    SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */
    SIM->SOPT2 |= 0x03000000; /* use MCGIRCLK as timer counter clock */
    TPM0->SC = 0; /* disable timer while configuring */
    TPM0->SC = 0x02; /* prescaler /4 ayuda a extender la cuenta del contador  */
    TPM0->MOD = time - 1; /* modulo value (32,768 Hz * 3 seg) / 4  */  
    //TPM0->CNT = time2 - 1;
    TPM0->SC |= 0x80; /* clear TOF */
    

    //Led azul externo
    PORTE->PCR[0] = 0x100;  /* make it GPIO */
    PTE->PDDR |= 0x1; /* make pin output */

    //Boton PTA17
    PORTA->PCR[17] = 0x103; /* make PTA17 GPIO & en pull-up */
    PTA->PDDR &= ~0x20000; /* make PTA17 as input pin */

    //Bandera de finalizacion de conteo
    bool conteoCoso = false;

    while (1) {
        
        //Obtener estado de conteo
        conteoCoso = conteo();

        //Si no se identifico sennal por periferico
        if (!conteoCoso) {
            while (true) {} //Mantenerse aqui
        }

        //Toggle Led verde despues de conteo en caso de siguiente fase
        PTB->PDOR |= 0x02;
        PTD->PDOR &= ~0x80000;
        delayMs(1000);
        PTB->PDOR |= 0x80000; /*Off*/

    
    }

}