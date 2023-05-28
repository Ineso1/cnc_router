#include "TpmMod.h"


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

void TpmMod::tpm0Config(){
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

void TpmMod::tpm1Config(){
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

void TpmMod::tpm2Config(){
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

void TpmMod::init(int tpmNum, int channel){
    this->tpmNum = tpmNum;
    this->channel = channel;
    SIM->SOPT2 |= 0x01000000;       //Enable TPM source
    __disable_irq();
    switch (tpmNum) {
    case 0:
        tpm0Config();
        break;
    case 1:
        tpm1Config();
        break;
    case 2:
        tpm2Config();
        break;
    default:
        break;
    }
    RgbConfig();
    RgbOff();
}

void TpmMod::sendPulses(uint32_t numPulses)
{

    pulse_counter = 0;
    target_pulses = numPulses;

    switch (tpmNum) {
        case 0:
            TPM0->CNT = 0;
            TPM0->MOD = target_pulses - 1; // Set modulo register
            TPM0->SC |= TPM_SC_TOIE_MASK;  // Enable TPM0 overflow interrupt
            break;
        case 1:
            TPM1->CNT = 0;
            TPM1->MOD = target_pulses - 1; // Set modulo register
            TPM1->SC |= TPM_SC_TOIE_MASK;  // Enable TPM1 overflow interrupt
            break;
        case 2:
            TPM2->CNT = 0;
            TPM2->MOD = target_pulses - 1; // Set modulo register
            TPM2->SC |= TPM_SC_TOIE_MASK;  // Enable TPM2 overflow interrupt
            break;
        default:
            break;
    }
}

void TpmMod::enablePort(char pinPort, int pin){
    this->pinPort = pinPort;
    this->pin = pin;
    switch (pinPort) {     // Set as TPM0_CH0
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            PORTA->PCR[pin] = PORT_PCR_MUX(3);
            PTA->PDDR &= ~(1<<pin);
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[pin] = PORT_PCR_MUX(3);
            PTB->PDDR &= ~(1<<pin);
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            if (pin < 7) {
                PORTC->PCR[pin] = PORT_PCR_MUX(4);
            }
            else {
                PORTC->PCR[pin] = PORT_PCR_MUX(3);
            }
            PTC->PDDR &= ~(1<<pin);
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            PORTD->PCR[pin] = PORT_PCR_MUX(4);
            PTD->PDDR &= ~(1<<pin);
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[pin] = PORT_PCR_MUX(3);
            PTE->PDDR &= ~(1<<pin);
            break;
        default:
            // Invalid port selection
            break;
    }
    switch (tpmNum) {
        case 0:
            tpm0Config();            // enable TPM0 interruption
            break;
        case 1:
            tpm1Config();
        case 2:
            tpm2Config();
            break;
        default:
            break;
    }
    
}

void TpmMod::disable(){
    switch (tpmNum) {
        case 0:
            TPM0->SC = 0;
            NVIC_DisableIRQ(TPM0_IRQn);
            break;
        case 1:
            TPM1->SC = 0;
            NVIC_DisableIRQ(TPM0_IRQn);
            break;
        case 2:
            TPM2->SC = 0;
            NVIC_DisableIRQ(TPM0_IRQn);
            break;
        default:
            TPM0->SC = 0;
            TPM1->SC = 0;
            TPM2->SC = 0;
            break;
        }
}


void TpmMod::enable(){
    edgeAlign();
    switch (tpmNum) {
        case 0:
            TPM0->SC |= TPM_SC_CMOD(1);  // Enable TPM0 counter
            break;
        case 1:
            TPM1->SC |= TPM_SC_CMOD(1);
            break;
        case 2:
            TPM2->SC |= TPM_SC_CMOD(1);
            break;
        default:
            break;
        }
}

void TpmMod::edgeAlign(){
    switch (tpmNum) {
        case 0:
            TPM0->CONTROLS[channel].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Enable edge-aligned PWM mode (MSB:MSA = 1:0, ELSB:ELSA = 1:0)
            break;
        case 1:
            TPM1->CONTROLS[channel].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Enable edge-aligned PWM mode (MSB:MSA = 1:0, ELSB:ELSA = 1:0)
            break;
        case 2:
            TPM2->CONTROLS[channel].CnSC = TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK;  // Enable edge-aligned PWM mode (MSB:MSA = 1:0, ELSB:ELSA = 1:0)
            break;
        default:
            break;
        }
}

void TpmMod::setModulo(int mod){
    switch (tpmNum) {
        case 0:
            TPM0->MOD = mod - 1;
            break;
        case 1:
            TPM1->MOD = mod - 1;
            break;
        case 2:
            TPM2->MOD = mod - 1;
            break;
        default:
            break;
    }
}

void TpmMod::setPrescaler(int prescaler){
    int ps_val;
    switch (prescaler) {
    case 1:
        ps_val = 0;
        break;
    case 2:
        ps_val = 1;
        break;
    case 4:
        ps_val = 2;
        break;
    case 8:
        ps_val = 3;
        break;
    case 16:
        ps_val = 4;
        break;
    case 32:
        ps_val = 5;
        break;
    case 64:
        ps_val = 6;
        break;
    case 128:
        ps_val = 7;
        break;
    default:
        ps_val = 7;
        break;
    }

    switch (tpmNum) {
        case 0:
            TPM0->SC = TPM_SC_PS(ps_val);
            break;
        case 1:
            TPM1->SC = TPM_SC_PS(ps_val);
            break;
        case 2:
            TPM2->SC = TPM_SC_PS(ps_val);
            break;
        default:
            break;
        }
}

void TpmMod::setChannelValue(int value){
    switch (tpmNum) {
        case 0:
            TPM0->CONTROLS[channel].CnV = value;
            break;
        case 1:
            TPM1->CONTROLS[channel].CnV = value;
            break;
        case 2:
            TPM2->CONTROLS[channel].CnV = value;
            break;
        default:
            break;
        }
}

bool TpmMod::available() {
    return pulse_counter + 1 >= target_pulses;
}



