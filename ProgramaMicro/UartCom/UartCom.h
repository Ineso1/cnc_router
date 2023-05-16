#ifndef UARTCOM_H
#define UARTCOM_H
#include <stdint.h>
#include <MKL25Z4.h>


class Uart{
    #define UART0_BAUD_RATE 9600
    #define UART0_CLOCK_SELECT 1
    #define UART0_OVERSAMPLING_RATIO 16

    public:
        Uart()
        void init();
        void sendChar(char c);
        void sendString(const char* str);
        void intToHex(uint32_t value, char* buffer, int bufferSize);
        void intToBinary(uint32_t value, char* buffer, int bufferSize);
        void sendCommand(uint32_t command);
        char readChar();
        void readString(char*, int, char); 
    private:

};

#endif  // UARTCOM_H