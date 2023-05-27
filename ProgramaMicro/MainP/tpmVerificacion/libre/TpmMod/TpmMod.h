#ifndef TPMMOD_H
#define TPMMOD_H

#include <MKL25Z4.h>

class TpmMod{


    private:
        int tpmNum = 0, channel = 0, pin = 0;
        char pinPort = 'A';

        void TPM0_IRQHandler();
        void TPM1_IRQHandler();
        void TPM2_IRQHandler();

    public:
        volatile uint32_t pulse_counter = 0;
        volatile uint32_t target_pulses = 0;

        void init(int tpmNum, int channel);
        void enablePort(char pinPort, int pin);
        void disable();
        void enable();
        void edgeAlign();
        void setModulo(int mod);
        void setPrescaler(int prescaler);
        void setChannelValue(int value);

        void sendPulses(uint32_t numPulses);

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
