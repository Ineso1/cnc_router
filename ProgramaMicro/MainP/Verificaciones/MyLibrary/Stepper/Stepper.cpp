#include "Stepper.h"


Stepper::Stepper(){}

/**
 * @brief Set the pins for controlling the stepper motor.
 * @param pin The pin number connected to the STEP signal of the stepper motor driver.
 * @param direction The pin number connected to the ENABLE signal of the stepper motor driver.
 * @param steps The number of steps per revolution of the stepper motor.
 * @param radius The radius of the pulley or gear attached to the stepper motor.
*/
void Stepper::setPins(char pinPort, int pin, char directionPort, int direction, int steps, int radius, int tpmN, int channel) {
    this->pinPort = pinPort;
    this->directionPort = directionPort;
    this->pin = pin;
    this->direction = (1 << direction);
    this->steps = steps;
    this->radius = radius;
    tpm.init(tpmN, channel);
    tpm.enablePort(pinPort, pin);

    // Set the pin and enable directions based on the selected port
    switch (pinPort) {
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            break;
        default:
            // Invalid port selection
            break;
    }

    switch (directionPort) {
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            PORTA->PCR[direction] = 0x100;
            PTA->PDDR |= this->direction;
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[direction] = 0x100;
            PTB->PDDR |= this->direction;
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            PORTC->PCR[direction] = 0x100;
            PTC->PDDR |= this->direction;
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            PORTD->PCR[direction] = 0x100;
            PTD->PDDR |= this->direction;
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[direction] = 0x100;
            PTE->PDDR |= this->direction;
            break;
        default:
            // Invalid port selection
            break;
    }
}

void Stepper::setDirection(bool direction){
    if (direction) {
        negativeDirection();
    }
    else {
        positiveDirection();
    }
}

void Stepper::setPulsesTarget(int stepsNum){
    tpm.target_pulses = stepsNum;
}

void Stepper::setTpmMod(int mod){
    tpm.setModulo(mod);
}

void Stepper::setChValue(int value){
    tpm.setChannelValue(value);
}

void Stepper::setTpmPrescaler(int prescaler){
    tpm.setPrescaler(prescaler);
}

void Stepper::clrPulseCounter(){
    tpm.pulse_counter = 0;
}

void Stepper::addPulseCounter(){
    tpm.pulse_counter++ ;
}

void Stepper::substractPulseCounter(){
    tpm.pulse_counter-- ;
}

void Stepper::setPulseDelay(int delay){
    pulseDelay = delay;
}

/**
 * @brief Getter function to retrieve the current position.
*/
int Stepper::getPosition() {
    return position;
}


int Stepper::getPulseCounter(){
    return tpm.pulse_counter;
}

int Stepper::getPulseTarget(){
    return tpm.target_pulses;
}

char Stepper::getPinPort(){
    return pinPort;
}

int8_t Stepper::getPin(){
    return pin;
}

bool Stepper::tpmProcess(){
    return tpm.available();
}


/**
 * @brief Disable the stepper motor by setting the direction pin to the appropriate level.
*/
void Stepper::negativeDirection(){
    switch (directionPort) {
        case 'A':
            PTA->PDOR &= ~(this->direction);
            break;
        case 'B':
            PTB->PDOR &= ~(this->direction);
            break;
        case 'C':
            PTC->PDOR &= ~(this->direction);
            break;
        case 'D':
            PTD->PDOR &= ~(this->direction);
            break;
        case 'E':
            PTE->PDOR &= ~(this->direction);
            break;
        default:
            // Invalid port selection
            break;
    }
}

/**
 * @brief Enable the stepper motor by setting the direction pin to the appropriate level.
*/
void Stepper::positiveDirection(){
    switch (directionPort) {
        case 'A':
            PTA->PDOR |= (this->direction);
            break;
        case 'B':
            PTB->PDOR |= (this->direction);
            break;
        case 'C':
            PTC->PDOR |= (this->direction);
            break;
        case 'D':
            PTD->PDOR |= (this->direction);
            break;
        case 'E':
            PTE->PDOR |= (this->direction);
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
            PTA->PDOR |= (1<<this->pin);
            delay_ms(pulseDelay);
            PTA->PDOR &= ~(1<<this->pin);
            delay_ms(pulseDelay);
            break;
        case 'B':
            PTB->PDOR |= (1<<this->pin);
            delay_ms(pulseDelay);
            PTB->PDOR &= ~(1<<this->pin);
            delay_ms(pulseDelay);
            break;
        case 'C':
            PTC->PDOR |= (1<<this->pin);
            delay_ms(pulseDelay);
            PTC->PDOR &= ~(1<<this->pin);
            delay_ms(pulseDelay);
            break;
        case 'D':
            PTD->PDOR |= (1<<this->pin);
            delay_ms(pulseDelay);
            PTD->PDOR &= ~(1<<this->pin);
            delay_ms(pulseDelay);
            break;
        case 'E':
            PTE->PDOR |= (1<<this->pin);
            delay_ms(pulseDelay);
            PTE->PDOR &= ~(1<<this->pin);
            delay_ms(pulseDelay);
            break;
        default:
            // Invalid port selection
            break;
    }
}




/* For tpm use */

void Stepper::enableTpm(){
    tpm.enable();
}

void Stepper::useTpm(){
    tpm.enablePort(pinPort, pin);
}

void Stepper::quitTpm(){
    tpm.disable();
    tpm.setModulo(0);
    tpm.setChannelValue(0);
    // Set the pin and enable directions based on the selected port
    switch (pinPort) {
        case 'A':
            SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
            PORTA->PCR[pin] = PORT_PCR_MUX(1);
            PTA->PDDR |= (1<<this->pin);
            break;
        case 'B':
            SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
            PORTB->PCR[pin] = PORT_PCR_MUX(1);
            PTB->PDDR |= (1<<this->pin);
            break;
        case 'C':
            SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
            PORTC->PCR[pin] = PORT_PCR_MUX(1);
            PTC->PDDR |= (1<<this->pin);
            break;
        case 'D':
            SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
            PORTD->PCR[pin] = PORT_PCR_MUX(1);
            PTD->PDDR |= (1<<this->pin);
            break;
        case 'E':
            SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
            PORTE->PCR[pin] = PORT_PCR_MUX(1);
            PTE->PDDR |= (1<<this->pin);
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

/**
 * @brief Calculate the number of steps required to move the stepper motor a specified distance in millimeters.
 * @param distance The distance to move in millimeters.
 * @return The number of steps required.
 */
int Stepper::calculateSteps_mm(float target) {
    int distance = target - position;
    if (distance < 0) {
        distance *= -1;
        setDirection(true);
    }
    else {
        setDirection(false);
    }
    return ((distance * steps) / (2.0 * PI * radius));
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
    quitTpm();
    if (distance < 0) {
        distance *= -1;
        setDirection(true);
    }
    else {
        setDirection(false);
    }
    int stepsToMove = ((distance * steps) / (2.0 * PI * radius));  // Calculate the number of steps to move based on distance steps and the gear radius

    for (int i = 0; i < stepsToMove; i++) {
        pulse();  // Generate a pulse for each step
    }
    position += distance; 
}


void Stepper::moveTo(int target) {
    
    useTpm();
    clrPulseCounter();
    int stepsToMove = calculateSteps_mm(target);
    setPulsesTarget(10);

    tpm.setChannelValue(32767);
    tpm.setModulo(65530);

    position = target;

    // Needs to enableTpm

}


void Stepper::moveTo(int target, int mod , int channelVal, int Npulses) {
    
    useTpm();
    clrPulseCounter();
    calculateSteps_mm(target);
    setPulsesTarget(Npulses);

    tpm.setChannelValue(channelVal);
    tpm.setModulo(mod);

    position = target;

    // Needs to enableTpm

}