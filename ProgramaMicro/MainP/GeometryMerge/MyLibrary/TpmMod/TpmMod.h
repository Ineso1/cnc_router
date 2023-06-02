/**
 * @file TpmMod.h
 * @brief Header file for the TpmMod class.
 */

#ifndef TPMMOD_H
#define TPMMOD_H

#include <MKL25Z4.h>
#include "MklTime.h"

/**
 * @brief Class for configuring and controlling TPM (Timer/PWM) modules.
 */
class TpmMod{

    private:
        int tpmNum = 0, channel = 0, pin = 0;
        char pinPort = 'A';

        void tpm0Config();
        void tpm1Config();
        void tpm2Config();

    public:
        volatile uint32_t pulse_counter = 0;
        volatile uint32_t target_pulses = 0;

        /**
        * @brief Initializes the TPM module and channel.
        * @param tpmNum The TPM module number (0, 1, or 2).
        * @param channel The TPM channel number (0-5).
        */
        void init(int tpmNum, int channel);

        /**
        * @brief Enables the specified port and pin for TPM operation.
        * @param pinPort The port of the pin ('A', 'B', 'C', 'D', or 'E').
        * @param pin The pin number.
        */
        void enablePort(char pinPort, int pin);

        /**
        * @brief Disables the TPM module and associated interrupts.
        */
        void disable();

        /**
        * @brief Enables the TPM module and starts the counter.
        */
        void enable();

        /**
        * @brief Configures the TPM module for edge-aligned PWM mode.
        */
        void edgeAlign();

        /**
        * @brief Sets the modulo value for the TPM module.
        * @param mod The modulo value.
        */
        void setModulo(int mod);

        /**
        * @brief Sets the prescaler value for the TPM module.
        * @param prescaler The prescaler value (1, 2, 4, 8, 16, 32, 64, or 128).
        */
        void setPrescaler(int prescaler);

        /**
        * @brief Sets the channel value for the TPM module.
        * @param value The channel value.
        */
        void setChannelValue(int value);

        /**
        * @brief Sends a specified number of pulses using the TPM module.
        * @param numPulses The number of pulses to send.
        */
        void sendPulses(uint32_t numPulses);

        /**
        * @brief Checks if all the requested pulses have been sent.
        * @return True if all the requested pulses have been sent, false otherwise.
        */
        bool available();

};

/*  ------------TPM0-------------------------------------------------

        A:  PTA0 - TPM0_CH5
            PTA3 - TPM0_CH0
            PTA4 - TPM0_CH1
            PTA5 - TPM0_CH2
        B: 
        C:  PTC1 - TPM0_CH0
            PTC2 - TPM0_CH1
            PTC3 - TPM0_CH2
            PTC4 - TPM0_CH3
            PTC8 - TPM0_CH4
            PTC9 - TPM0_CH5
        D:  PTD0 - TPM0_CH0
            PTD1 - TPM0_CH1
            PTD2 - TPM0_CH2
            PTD3 - TPM0_CH3
            PTD4 - TPM0_CH4
            PTD5 - TPM0_CH5
        
        E:  PTE29 - TPM0_CH2
            PTE30 - TPM0_CH3
            PTE31 - TPM0_CH4
            PTE24 - TPM0_CH0
            PTE25 - TPM0_CH1
        */

/*  ------------TPM1-------------------------------------------------
        A:  PTA12 - TPM1_CH0
            PTA13 - TPM1_CH1
        B:  PTB0 - TPM1_CH0
            PTB1 - TPM1_CH1
        C: 
        D: 
        E:  PTE20 - TPM1_CH0
            PTE21 - TPM1_CH1  
*/


/*  ------------TPM2-------------------------------------------------
        A:  PTA1 - TPM2_CH0
            PTA2 - TPM2_CH1
        B:  PTB2 - TPM2_CH0
            PTB3 - TPM2_CH1 
            PTB18 - TPM2_CH0
            PTB19 - TPM2_CH1  
        C: 
        D: 
        E:  PTE22 - TPM2_CH0
            PTE23 - TPM2_CH1
*/

#endif // TPMMOD_H
