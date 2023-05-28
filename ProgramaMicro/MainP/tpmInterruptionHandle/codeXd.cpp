// #include <MKL25Z4.h>
// #include "MklTime.h"

// int main (void) {
// __disable_irq(); /* global disable IRQs */

// SIM->SCGC5 |= 0x400; /* enable clock to Port B */
// SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
// PORTB->PCR[18] = 0x100; /* make PTB18 pin as GPIO */
// PTB->PDDR |= 0x40000; /* make PTB18 as output pin */
// PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */
// PTB->PDDR |= 0x80000; /* make PTB19 as output pin */
// PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
// PTD->PDDR |= 0x02; /* make PTD1 as output pin */


// /* Configuring first timer*/
// /* use MCGFLLCLK as timer counter clock */
// SIM->SOPT2 |= 0x01000000; 
// SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */

// TPM0->SC = 0; /* disable timer while configuring */
// TPM0->SC = 0x07; /* prescaler /128 */
// TPM0->MOD = 0xFFFF; /* max modulo value */
// TPM0->SC |= 0x80; /* clear TOF */
// TPM0->SC |= 0x40; /* enable timeout interrupt */
// TPM0->SC |= 0x08; /* enable timer */
// NVIC->ISER[0] |= 0x00020000; 
// /* enable IRQ17 (bit 17 of ISER[0]) */
// /* Configuring Second Timer */
// SIM->SCGC6 |= 0x02000000; /* enable clock to TPM1 */
// TPM1->SC = 0; /* disable timer while configuring */
// TPM1->SC = 0x07; /* prescaler /128 */
// TPM1->MOD = 0x7FFF; /* half of the max modulo value */
// TPM1->SC |= 0x40; /* enable timeout interrupt */
// TPM1->SC |= 0x08; /* enable timer */
// NVIC->ISER[0] |= 0x00040000; 
// /* enable IRQ18 (bit 18 of ISER[0]) */

// __enable_irq(); /* global enable IRQs */
// while (1) {
// PTD->PTOR = 0x02; /* toggle blue LED */
// delay_ms(1500);  } }


// extern "C" void TPM0_IRQHandler(void) {
// PTB->PTOR = 0x40000; /* toggle red LED */
// TPM0->SC |= 0x80; /* clear TOF */
// }
// extern "C" void TPM1_IRQHandler(void) {
// PTB->PTOR = 0x80000; /* toggle green LED */
// TPM1->SC |= 0x80; /* clear TOF */
// }