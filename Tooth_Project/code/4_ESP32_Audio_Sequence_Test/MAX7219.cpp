/*
 * MAX7219 RTC Module
 */
#include <SPI.h> 
#include "MAX7219.h"
#include <Arduino.h>

void spiWriteRegister(uint8_t reg, uint8_t value) { 
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)); 
  digitalWrite(CS, LOW);  // start transfer 
  SPI.transfer(reg);  // Send address.
  SPI.transfer(value);  // Send the value.
  digitalWrite(CS, HIGH);  // Finish transfer.
  SPI.endTransaction();
}


