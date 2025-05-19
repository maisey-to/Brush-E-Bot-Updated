#include "DFRobotDFPlayerMini.h"

// *** DEBUG RESOURCES *** //
#define DEBUG
#define DEBUG_MUSIC

// *** OpenLog Pins *** //
#define UART_TX 22
#define UART_RX 21

// *** RTC Pins *** //
#define I2C_SDA 13
#define I2C_SCL 26

// *** DF Player Pins *** //
#define RXD2 16
#define TXD2 17

// *** Eye LedControl pins *** //
#define DIN_LEFT 23
#define MISO_LEFT 19
#define CS_LEFT 5
#define CLK_LEFT 18

// *** Analog Pin for RandomSeed *** //
// TODO: Verify that this pin works for reading analog for random seed generation
#define ANALOG_PIN 15


// Function from DFPlayerMini code to print out debug info
void printDetail(uint8_t type, int value)
{
    switch (type) {
      case TimeOut:
        Serial.println(F("Time Out!"));
        break;
      case WrongStack:
        Serial.println(F("Stack Wrong!"));
        break;
      case DFPlayerCardInserted:
        Serial.println(F("Card Inserted!"));
        break;
      case DFPlayerCardRemoved:
        Serial.println(F("Card Removed!"));
        break;
      case DFPlayerCardOnline:
        Serial.println(F("Card Online!"));
        break;
      case DFPlayerUSBInserted:
        Serial.println("USB Inserted!");
        break;
      case DFPlayerUSBRemoved:
        Serial.println("USB Removed!");
        break;
      case DFPlayerPlayFinished:
        Serial.print(F("Number:"));
        Serial.print(value);
        Serial.println(F(" Play Finished!"));
        break;
      case DFPlayerError:
        Serial.print(F("DFPlayerError:"));
        switch (value) {
          case Busy:
            Serial.println(F("Card not found"));
            break;
          case Sleeping:
            Serial.println(F("Sleeping"));
            break;
          case SerialWrongStack:
            Serial.println(F("Get Wrong Stack"));
            break;
          case CheckSumNotMatch:
            Serial.println(F("Check Sum Not Match"));
            break;
          case FileIndexOut:
            Serial.println(F("File Index Out of Bound"));
            break;
          case FileMismatch:
            Serial.println(F("Cannot Find File"));
            break;
          case Advertise:
            Serial.println(F("In Advertise"));
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
}