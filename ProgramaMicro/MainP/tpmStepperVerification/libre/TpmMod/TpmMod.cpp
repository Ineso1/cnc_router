#include "TpmMod.h"

void TpmMod::init(int tpmNum, int channel){
    this->tpmNum = tpmNum;
    this->channel = channel;
    SIM->SOPT2 |= 0x01000000;
    __disable_irq();
    switch (tpmNum) {
    case 0:
        SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;  // Enable TPM0 clock
        TPM0->SC = 0;
        NVIC_SetPriority(TPM0_IRQn, 2);       // priority of TPM0 interruption
        NVIC_EnableIRQ(TPM0_IRQn);             // enable TPM0 interruption

        break;
    case 1:
        SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;  // Enable TPM1 clock
        TPM1->SC = 0;
        NVIC_SetPriority(TPM1_IRQn, 2);       // priority of TPM1 interruption
        NVIC_EnableIRQ(TPM1_IRQn);             // enable TPM1 interruption

        break;
    case 2:
        SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;  // Enable TPM1 clock
        TPM2->SC = 0;
        NVIC_SetPriority(TPM2_IRQn, 2);       // priority of TPM2 interruption
        NVIC_EnableIRQ(TPM2_IRQn);             // enable TPM2 interruption

        break;
    default:
        break;
    }
    setPrescaler(128);
}

extern "C" void TpmMod::TPM_IRQHandler() {
    switch (tpmNum) {
        case 0:
            TPM0->SC |= TPM_SC_TOF_MASK; // Clear TPM0 overflow flag
            break;
        case 1:
            TPM1->SC |= TPM_SC_TOF_MASK; // Clear TPM1 overflow flag
            break;
        case 2:
            TPM2->SC |= TPM_SC_TOF_MASK; // Clear TPM2 overflow flag
            break;
        default:
            break;
    }

    pulse_counter++;

    if (target_pulses <= pulse_counter) {
        // Disable TPM module by clearing the clock gate
        switch (tpmNum) {
            case 0:
                SIM->SCGC6 &= ~SIM_SCGC6_TPM0_MASK;
                TPM0->CONTROLS[channel].CnV = 0;
                break;
            case 1:
                SIM->SCGC6 &= ~SIM_SCGC6_TPM1_MASK;
                TPM1->CONTROLS[channel].CnV = 0;
                break;
            case 2:
                SIM->SCGC6 &= ~SIM_SCGC6_TPM2_MASK;
                TPM2->CONTROLS[channel].CnV = 0;
                break;
            default:
                break;
        }
    }
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
            TPM0->SC |= TPM_SC_TOIE_MASK; 
            NVIC_SetPriority(TPM0_IRQn, 2);       // priority of TPM0 interruption
            NVIC_EnableIRQ(TPM0_IRQn);             // enable TPM0 interruption
            break;
        case 1:
            TPM1->SC |= TPM_SC_TOIE_MASK; 
            NVIC_SetPriority(TPM1_IRQn, 2);       // priority of TPM1 interruption
            NVIC_EnableIRQ(TPM1_IRQn);             // enable TPM1 interruption
            break;
        case 2:
            TPM2->SC |= TPM_SC_TOIE_MASK; 
            NVIC_SetPriority(TPM2_IRQn, 2);       // priority of TPM2 interruption
            NVIC_EnableIRQ(TPM2_IRQn);             // enable TPM2 interruption
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
    return pulse_counter >= target_pulses;
}