/*
 * MAX7219 LED Matrix
 */

#ifndef MAX7219_H
#define MAX7219_H

// MAX7219 Registers
#define DECODE_MODE     0x09 
#define INTENSITY       0x0A
#define SCAN_LIMIT      0x0B 
#define SHUTDOWN        0x0C
#define DISPLAY_TEST    0x0F

#define CS 5

void spiWriteRegister(uint8_t reg, uint8_t value);

#endif /* MAX7219_H */