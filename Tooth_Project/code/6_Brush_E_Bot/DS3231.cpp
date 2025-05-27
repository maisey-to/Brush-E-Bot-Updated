/*
 * DS3231 RTC Module
 */
#include <Wire.h> 
#include "DS3231.h"

uint8_t readRegister(uint8_t reg){
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // Send command
  Wire.endTransmission();  // Stop bit

  // Request 1 byte
  Wire.requestFrom(DS3231_ADDR, 1);
  return Wire.read();  // read and return 1 byte
}

uint8_t readTimeRegister(uint8_t reg){
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // send command
  Wire.endTransmission();  // Stop bit

  // Request 1 byte
  Wire.requestFrom(DS3231_ADDR, 1);
  return _fromBcd(Wire.read());  // read and return 1 byte, convert BCD to int
}

uint8_t writeRegister(uint8_t reg, uint8_t data){
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // send command
  Wire.write(data);  // send data
  Wire.endTransmission();  // Stop bit
}

uint8_t writeTimeRegister(uint8_t reg, uint8_t data){
  Wire.beginTransmission(DS3231_ADDR);  // Start bit
  Wire.write(reg);  // send command
  Wire.write(_toBcd(data));  // send data, convert int to BCD
  Wire.endTransmission();  // Stop bit
}

uint8_t _toBcd(uint8_t num)
{
	uint8_t bcd = ((num / 10) << 4) + (num % 10);
	return bcd;
}

uint8_t _fromBcd(uint8_t bcd) {
	uint8_t num = (10*((bcd&0xf0) >>4)) + (bcd & 0x0f);
	return num;
}

