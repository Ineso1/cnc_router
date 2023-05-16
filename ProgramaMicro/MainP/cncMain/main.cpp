// #include "mbed.h"
#include <MKL25Z4.h>
#include "UartCom.h"


void delay_ms(int n) {
    int i;
    SysTick->LOAD = 41940 - 1;
    SysTick->CTRL = 0x5; /* Enable the timer and choose sysclk as the clock
    source */
    for(i = 0; i < n; i++) {
        while((SysTick->CTRL & 0x10000) == 0) /* wait until the COUNT flag is set */
        { }
    }
    SysTick->CTRL = 0;
    /* Stop the timer (Enable = 0) */
}

// main() runs in its own thread in the OS
int main()
{
    UART0_Init();
    while (true) {
    UART0_SendString("jalese compa\n");
    delay_ms(1000);
    }
}

