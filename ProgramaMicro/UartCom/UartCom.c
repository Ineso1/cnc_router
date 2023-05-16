#include "UartCom.h"

void UART0_Init() {
    // Enable clock to UART0 module
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    // Configure UART0 pins
    PORTA->PCR[1] = PORT_PCR_MUX(2); // UART0_RX (PTA1)
    PORTA->PCR[2] = PORT_PCR_MUX(2); // UART0_TX (PTA2)
    
    // Disable transmitter and receiver while configuring UART0
    UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    
    // Calculate UART0 baud rate divisor
    uint16_t sbr = (uint16_t)((SystemCoreClock / UART0_CLOCK_SELECT) / (UART0_BAUD_RATE * UART0_OVERSAMPLING_RATIO));
    
    // Set UART0 baud rate divisor
    UART0->BDH = (UART0->BDH & ~UART_BDH_SBR_MASK) | UART_BDH_SBR(sbr >> 8);
    UART0->BDL = (UART0->BDL & ~UART_BDL_SBR_MASK) | UART_BDL_SBR(sbr);
    
    // Enable transmitter and receiver
    UART0->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
}

void UART0_SendChar(char c) {
    // Wait until transmit data register is empty
    while (!(UART0->S1 & UART_S1_TDRE_MASK));
    
    // Send character
    UART0->D = c;
}

void UART0_SendString(const char* str) {
    while (*str) {
        UART0_SendChar(*str);
        str++;
    }
}

void UART0_IntToHex(uint32_t value, char* buffer, int bufferSize) {
    const char hexChars[] = "0123456789ABCDEF";
    int i = bufferSize - 1;
    
    buffer[i] = '\0';

    // Process each nibble of the value
    for (; i > 0 && value != 0; i--) {
        buffer[i - 1] = hexChars[value & 0x0F];
        value >>= 4;
    }

    // Add leading zeros if necessary
    while (i > 0) {
        buffer[i - 1] = '0';
        i--;
    }
}

void UART0_IntToBinary(uint32_t value, char* buffer, int bufferSize) {
    int i = bufferSize - 1;

    buffer[i] = '\0';

    // Process each bit of the value
    for (; i > 0 && value != 0; i--) {
        buffer[i - 1] = (value & 1) ? '1' : '0';
        value >>= 1;
    }

    // Add leading zeros if necessary
    while (i > 0) {
        buffer[i - 1] = '0';
        i--;
    }
}

void UART0_SendCommand(uint32_t command) {
    // Convert the hexadecimal command to a string
    char hexCommand[9];
    UART0_IntToHex(command, hexCommand, sizeof(hexCommand));
    
    // Send the command string
    UART0_SendString(hexCommand);
}