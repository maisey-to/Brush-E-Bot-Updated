/*
 * DS3231 RTC Module
 */

#ifndef DS3231_H
#define DS3231_H

// RTC Registers
#define DS3231_ADDR             0x68  ///< I2C address for DS3231
#define SD3231_READ             0xD1
#define SD3231_WRITE            0xD0

#define DS3231_SECONDS          0x00  
#define DS3231_MINUTES          0x01  
#define DS3231_HOURS            0x02  
#define DS3231_DAY              0x03  
#define DS3231_DATE             0x04  
#define DS3231_CEN_MONTH        0x05
#define DS3231_DEC_YEAR         0x06
#define DS3231_ALARM1_SECONDS   0x07
#define DS3231_ALARM1_MINUTES   0x08
#define DS3231_ALARM1_HOURS     0x09
#define DS3231_ALARM1_DAY_DATE  0x0A
#define DS3231_ALARM2_MINUTES   0x0B
#define DS3231_ALARM2_HOURS     0x0C
#define DS3231_ALARM2_DAY_DATE  0x0D
#define DS3231_CONTROL          0x0E
#define DS3231_CTL_STATUS       0x0F
#define DS3231_AGING_OFFSET     0x10
#define DS3231_TEMP_MSB         0x11
#define DS3231_TEMP_LSB         0x12

uint8_t readRegister(uint8_t reg);
uint8_t readTimeRegister(uint8_t reg);
uint8_t writeRegister(uint8_t reg, uint8_t data);
uint8_t writeTimeRegister(uint8_t reg, uint8_t data);
uint8_t _toBcd(uint8_t num);
uint8_t _fromBcd(uint8_t bcd);

#endif /* DS3231_H */