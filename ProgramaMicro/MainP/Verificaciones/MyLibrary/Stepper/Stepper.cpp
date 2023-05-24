#include "Stepper.h"
#include <cstdint>


Stepper::Stepper(){}

/**
 * @brief Set the pins for controlling the stepper motor.
 * @param pin The pin number connected to the STEP signal of the stepper motor driver.
 * @param enable The pin number connected to the ENABLE signal of the stepper motor driver.
 * @param steps The number of steps per revolution of the stepper motor.
 * @param radius The radius of the pulley or gear attached to the stepper motor.
*/
void Stepper::setPins(char pinPort, int pin, char enablePort, int enable, int steps, int radius) {
    this->pinPort = pinPort;
    this->enablePort = enablePort;
    this->pin = (1 << pin);
    this->enable = (1 << enable);
    this->steps = steps;
    this->radius = radius;

    // Set the pin and enable directions based on the selected port
    switch (pinPort) {
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            PORTA->PCR[pin] = 0x100;
            PTA->PDDR |= this->pin;
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[pin] = 0x100;
            PTB->PDDR |= this->pin;
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            PORTC->PCR[pin] = 0x100;
            PTC->PDDR |= this->pin;
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            PORTD->PCR[pin] = 0x100;
            PTD->PDDR |= this->pin;
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[pin] = 0x100;
            PTE->PDDR |= this->pin;
            break;
        default:
            // Invalid port selection
            break;
    }

    switch (enablePort) {
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            PORTA->PCR[enable] = 0x100;
            PTA->PDDR |= this->enable;
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[enable] = 0x100;
            PTB->PDDR |= this->enable;
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            PORTC->PCR[enable] = 0x100;
            PTC->PDDR |= this->enable;
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            PORTD->PCR[enable] = 0x100;
            PTD->PDDR |= this->enable;
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[enable] = 0x100;
            PTE->PDDR |= this->enable;
            break;
        default:
            // Invalid port selection
            break;
    }
}

char Stepper::getPinPort(){
    return pinPort;
}

int8_t Stepper::getPin(){
    return pin;
}

/**
 * @brief Disable the stepper motor by setting the ENABLE pin to the appropriate level.
*/
void Stepper::disableMotor(){
    switch (enablePort) {
        case 'A':
            PTA->PDOR &= ~(this->enable);
            break;
        case 'B':
            PTB->PDOR &= ~(this->enable);
            break;
        case 'C':
            PTC->PDOR &= ~(this->enable);
            break;
        case 'D':
            PTD->PDOR &= ~(this->enable);
            break;
        case 'E':
            PTE->PDOR &= ~(this->enable);
            break;
        default:
            // Invalid port selection
            break;
    }
}


/**
 * @brief Enable the stepper motor by setting the ENABLE pin to the appropriate level.
*/
void Stepper::enableMotor(){
    switch (enablePort) {
        case 'A':
            PTA->PDOR |= (this->enable);
            break;
        case 'B':
            PTB->PDOR |= (this->enable);
            break;
        case 'C':
            PTC->PDOR |= (this->enable);
            break;
        case 'D':
            PTD->PDOR |= (this->enable);
            break;
        case 'E':
            PTE->PDOR |= (this->enable);
            break;
        default:
            // Invalid port selection
            break;
    }
}

/**
 * @brief Generate a pulse on the STEP pin to move the stepper motor by one step.
*/
void Stepper::pulse(){
    switch (pinPort) {
        case 'A':
            PTA->PDOR |= (this->pin);
            delay_ms(pulseDelay);
            PTA->PDOR &= ~(this->pin);
            delay_ms(pulseDelay);
            break;
        case 'B':
            PTB->PDOR |= (this->pin);
            delay_ms(pulseDelay);
            PTB->PDOR &= ~(this->pin);
            delay_ms(pulseDelay);
            break;
        case 'C':
            PTC->PDOR |= (this->pin);
            delay_ms(pulseDelay);
            PTC->PDOR &= ~(this->pin);
            delay_ms(pulseDelay);
            break;
        case 'D':
            PTD->PDOR |= (this->pin);
            delay_ms(pulseDelay);
            PTD->PDOR &= ~(this->pin);
            delay_ms(pulseDelay);
            break;
        case 'E':
            PTE->PDOR |= (this->pin);
            delay_ms(pulseDelay);
            PTE->PDOR &= ~(this->pin);
            delay_ms(pulseDelay);
            break;
        default:
            // Invalid port selection
            break;
    }
}

/*
    N pulsos -> 1rev

    steps -> Diametro * pi
    steps? -> distancia

    distancia * steps / (2*radius*pi)
*/

/**
 * @brief Move the stepper motor a specified distance in millimeters.
 * @param distance The distance to move in millimeters.
*/
void Stepper::move_mm(float distance){
    int stepsToMove = ((distance * steps) / (2.0 * PI * radius));  // Calculate the number of steps to move based on distance steps and the gear radius

    for (int i = 0; i < stepsToMove; i++) {
        pulse();  // Generate a pulse for each step
    }
}

/**
 * @brief Getter function to retrieve the current position.
*/
int Stepper::getPosition() {
    return position;
}

void Stepper::setPulseDelay(int delay){
    pulseDelay = delay;
}







/* Para usar tpm */

/*


period = 2 * (delayTime + t_off)

where
delayTime is the desired delay between each step.
t_off is the time required for the motor to turn off before taking the next step. This value depends on the motor specifications and driver circuitry and can often be neglected or set to a small value.

For example, let's say you have a delay time of 1 millisecond between each step and you neglect the turn-off time. The calculation would be: period = 2 * (1 + 0) = 2 milliseconds
This means the TMP module should generate a pulse every 2 milliseconds.

To set the duty cycle, you can use the following formula:
dutyCycle = (pulseDuration / period) * 100

For example, if you want a 50% duty cycle, the pulse duration would be half of the period:
pulseDuration = period / 2 = 2 milliseconds / 2 = 1 millisecond


*/

void Stepper::useTpm(){
    switch (pinPort) {     // Set as TPM0_CH0
        case 'A':
            PORTA->PCR[pin] = PORT_PCR_MUX(3);
            break;
        case 'B':
            PORTB->PCR[pin] = PORT_PCR_MUX(3);
            break;
        case 'C':
            PORTC->PCR[pin] = PORT_PCR_MUX(3);
            break;
        case 'D':
            PORTD->PCR[pin] = PORT_PCR_MUX(3);
            break;
        case 'E':
            PORTE->PCR[pin] = PORT_PCR_MUX(3);
            break;
        default:
            // Invalid port selection
            break;
    }
}

void Stepper::quitTpm(){
    // Set the pin and enable directions based on the selected port
    switch (pinPort) {
        case 'A':
            PORTA->PCR[pin] = 0x100;
            PTA->PDDR |= this->pin;
            break;
        case 'B':
            PORTB->PCR[pin] = 0x100;
            PTB->PDDR |= this->pin;
            break;
        case 'C':
            PORTC->PCR[pin] = 0x100;
            PTC->PDDR |= this->pin;
            break;
        case 'D':
            PORTD->PCR[pin] = 0x100;
            PTD->PDDR |= this->pin;
            break;
        case 'E':
            PORTE->PCR[pin] = 0x100;
            PTE->PDDR |= this->pin;
            break;
        default:
            // Invalid port selection
            break;
    }
}

int Stepper::calculatePeriod(int delayTime) {
        int t_off = 0; // Adjust this 
        return 2 * (delayTime + t_off);
    }

int Stepper::calculatePulseDuration(int period) {
        return period / 2;
    }

void Stepper::move_mm(float distance, int delayTime) {
        int stepsToMove = ((distance * steps) / (2.0 * PI * radius));
        int period = calculatePeriod(delayTime);
        int pulseDuration = calculatePulseDuration(period);

        // Set TPM0 period and pulse duration
        // TPM0_MOD = period;
        // TPM0_C0V = pulseDuration;

        // Enable TPM0 counter

        // Generate pulses for each step
        for (int i = 0; i < stepsToMove; i++) {
            pulse();
            delay_ms(delayTime);
        }

        // Disable TPM0 counter and reset TPM0 channel 0 to default settings
        // TPM0_SC = 0;
        // TPM0_C0SC = 0;


}