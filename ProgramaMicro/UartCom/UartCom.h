#ifndef SERIALCOM_H
#define SERIALCOM_H
#include <stdint.h>
#include <MKL25Z4.h>

#define UART0_BAUD_RATE 9600
#define UART0_CLOCK_SELECT 1
#define UART0_OVERSAMPLING_RATIO 16

void UART0_Init();
void UART0_SendChar(char c);
void UART0_SendString(const char* str);
void UART0_IntToHex(uint32_t value, char* buffer, int bufferSize);
void UART0_IntToBinary(uint32_t value, char* buffer, int bufferSize);
void UART0_SendCommand(uint32_t command);
#endif  // SERIALCOM_H