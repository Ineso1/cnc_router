#include "TpmMod.h"

void TpmMod::init(int tpmNum, int channel){
    this->tpmNum = tpmNum;
    this->channel = channel;
    SIM->SOPT2 |= 0x01000000;

    switch (tpmNum) {
    case 0:
        SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // Enable TPM0 clock
        TPM0->SC = 0;

        break;
    case 1:
        SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // Enable TPM1 clock
        TPM1->SC = 0;
        break;
    case 2:
        SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;  // Enable TPM1 clock
        TPM2->SC = 0;
        break;
    default:
        break;
    }
    setPrescaler(128);

}

void TpmMod::enablePort(char pinPort, int pin){
    this->pinPort = pinPort;
    this->pin = pin;
    switch (pinPort) {     // Set as TPM0_CH0
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            PORTA->PCR[pin] = PORT_PCR_MUX(3);
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[pin] = PORT_PCR_MUX(3);
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            if (pin < 7) {
                PORTC->PCR[pin] = PORT_PCR_MUX(4);
            }
            else {
                PORTC->PCR[pin] = PORT_PCR_MUX(3);
            }
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            PORTD->PCR[pin] = PORT_PCR_MUX(4);
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[pin] = PORT_PCR_MUX(3);
            break;
        default:
            // Invalid port selection
            break;
    }
}

void TpmMod::disable(){
    switch (tpmNum) {
        case 0:
            TPM0->SC = 0;
            break;
        case 1:
            TPM1->SC = 0;
            break;
        case 2:
            TPM2->SC = 0;
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
            TPM0->MOD = mod;
            break;
        case 1:
            TPM1->MOD = mod;
            break;
        case 2:
            TPM2->MOD = mod;
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