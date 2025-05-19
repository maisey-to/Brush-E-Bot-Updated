// https://xantorohara.github.io/led-matrix-editor
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include "MAX7219.h"

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

// *** Use UART1 for RTC *** //
HardwareSerial openLog(1); 

// *** Define globals *** //
#define FRAME_RATE 250  // ms
#define MAX_COUNTER 10
// #define NEXT_SECTION_AUDIO 1


uint8_t gSecs = 0;
uint8_t gCounter = 0;

// *** Music Global Variables *** //
long gFolderNumber;
uint8_t gSongIndex = 2; // Sound effect will be song /001, so music files start at /002
uint8_t NEXT_SECTION_AUDIO = 1;

// *** DF Player Serial *** //
DFRobotDFPlayerMini music;

const uint8_t NUMBERS[][10] = {
  {0b00000000, 0b00001110, 0b00001010, 0b00001010, 0b00001010, 0b00001010, 0b00001110, 0b00000000},
  {0b00000000, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b00000000},
  {0b00000000, 0b00001110, 0b00000010, 0b00001110, 0b00001000, 0b00001000, 0b00001110, 0b00000000},
  {0b00000000, 0b00001110, 0b00000010, 0b00001110, 0b00000010, 0b00000010, 0b00001110, 0b00000000},
  {0b00000000, 0b00001010, 0b00001010, 0b00001110, 0b00000010, 0b00000010, 0b00000010, 0b00000000},
  {0b00000000, 0b00001110, 0b00001000, 0b00001110, 0b00000010, 0b00000010, 0b00001110, 0b00000000},
  {0b00000000, 0b00001110, 0b00001000, 0b00001110, 0b00001010, 0b00001010, 0b00001110, 0b00000000},
  {0b00000000, 0b00001110, 0b00001010, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000000},
  {0b00000000, 0b00001110, 0b00001010, 0b00001110, 0b00001010, 0b00001010, 0b00001110, 0b00000000},
  {0b00000000, 0b00001110, 0b00001010, 0b00001110, 0b00000010, 0b00000010, 0b00001110, 0b00000000},
  {0b00000000, 0b00101110, 0b00101010, 0b00101010, 0b00101010, 0b00101010, 0b00101110, 0b00000000},
  {0b00000000, 0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00100100, 0b00000000},
  {0b00000000, 0b00101110, 0b00100010, 0b00101110, 0b00101000, 0b00101000, 0b00101110, 0b00000000},
  {0b00000000, 0b00101110, 0b00100010, 0b00101110, 0b00100010, 0b00100010, 0b00101110, 0b00000000},
  {0b00000000, 0b00101010, 0b00101010, 0b00101110, 0b00100010, 0b00100010, 0b00100010, 0b00000000},
  {0b00000000, 0b00101110, 0b00101000, 0b00101110, 0b00100010, 0b00100010, 0b00101110, 0b00000000},
  {0b00000000, 0b00101110, 0b00101000, 0b00101110, 0b00101010, 0b00101010, 0b00101110, 0b00000000},
  {0b00000000, 0b00101110, 0b00101010, 0b00100010, 0b00100010, 0b00100010, 0b00100010, 0b00000000},
  {0b00000000, 0b00101110, 0b00101010, 0b00101110, 0b00101010, 0b00101010, 0b00101110, 0b00000000},
  {0b00000000, 0b00101110, 0b00101010, 0b00101110, 0b00100010, 0b00100010, 0b00101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11101010, 0b10001010, 0b10001010, 0b11101110, 0b00000000},
  {0b00000000, 0b11100100, 0b00100100, 0b11100100, 0b10000100, 0b10000100, 0b11100100, 0b00000000},
  {0b00000000, 0b11101110, 0b00100010, 0b11101110, 0b10001000, 0b10001000, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00100010, 0b11101110, 0b10000010, 0b10000010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101010, 0b00101010, 0b11101110, 0b10000010, 0b10000010, 0b11100010, 0b00000000},
  {0b00000000, 0b11101110, 0b00101000, 0b11101110, 0b10000010, 0b10000010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101000, 0b11101110, 0b10001010, 0b10001010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11100010, 0b10000010, 0b10000010, 0b11100010, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11101110, 0b10001010, 0b10001010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11101110, 0b10000010, 0b10000010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11101010, 0b00101010, 0b00101010, 0b11101110, 0b00000000},
  {0b00000000, 0b11100100, 0b00100100, 0b11100100, 0b00100100, 0b00100100, 0b11100100, 0b00000000},
  {0b00000000, 0b11101110, 0b00100010, 0b11101110, 0b00101000, 0b00101000, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00100010, 0b11101110, 0b00100010, 0b00100010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101010, 0b00101010, 0b11101110, 0b00100010, 0b00100010, 0b11100010, 0b00000000},
  {0b00000000, 0b11101110, 0b00101000, 0b11101110, 0b00100010, 0b00100010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101000, 0b11101110, 0b00101010, 0b00101010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11100010, 0b00100010, 0b00100010, 0b11100010, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11101110, 0b00101010, 0b00101010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b00101010, 0b11101110, 0b00100010, 0b00100010, 0b11101110, 0b00000000},
  {0b00000000, 0b10101110, 0b10101010, 0b11101010, 0b00101010, 0b00101010, 0b00101110, 0b00000000},
  {0b00000000, 0b10100100, 0b10100100, 0b11100100, 0b00100100, 0b00100100, 0b00100100, 0b00000000},
  {0b00000000, 0b10101110, 0b10100010, 0b11101110, 0b00101000, 0b00101000, 0b00101110, 0b00000000},
  {0b00000000, 0b10101110, 0b10100010, 0b11101110, 0b00100010, 0b00100010, 0b00101110, 0b00000000},
  {0b00000000, 0b10101010, 0b10101010, 0b11101110, 0b00100010, 0b00100010, 0b00100010, 0b00000000},
  {0b00000000, 0b10101110, 0b10101000, 0b11101110, 0b00100010, 0b00100010, 0b00101110, 0b00000000},
  {0b00000000, 0b10101110, 0b10101000, 0b11101110, 0b00101010, 0b00101010, 0b00101110, 0b00000000},
  {0b00000000, 0b10101110, 0b10101010, 0b11100010, 0b00100010, 0b00100010, 0b00100010, 0b00000000},
  {0b00000000, 0b10101110, 0b10101010, 0b11101110, 0b00101010, 0b00101010, 0b00101110, 0b00000000},
  {0b00000000, 0b10101110, 0b10101010, 0b11101110, 0b00100010, 0b00100010, 0b00100010, 0b00000000},
  {0b00000000, 0b11101110, 0b10001010, 0b11101010, 0b00101010, 0b00101010, 0b11101110, 0b00000000},
  {0b00000000, 0b11100100, 0b10000100, 0b11100100, 0b00100100, 0b00100100, 0b11100100, 0b00000000},
  {0b00000000, 0b11101110, 0b10000010, 0b11101110, 0b00101000, 0b00101000, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b10000010, 0b11101110, 0b00100010, 0b00100010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101010, 0b10001010, 0b11101110, 0b00100010, 0b00100010, 0b11100010, 0b00000000},
  {0b00000000, 0b11101110, 0b10001000, 0b11101110, 0b00100010, 0b00100010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b10001000, 0b11101110, 0b00101010, 0b00101010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b10001010, 0b11100010, 0b00100010, 0b00100010, 0b11100010, 0b00000000},
  {0b00000000, 0b11101110, 0b10001010, 0b11101110, 0b00101010, 0b00101010, 0b11101110, 0b00000000},
  {0b00000000, 0b11101110, 0b10001010, 0b11101110, 0b00100010, 0b00100010, 0b11100010, 0b00000000}
};  // Numbers from 0 to 59

const int NUMBERS_LEN = sizeof(NUMBERS)/8;

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

void setup() {
  // Serial monitor output
  Serial.begin(9600);

  // Init OpenLog
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

  // I2C init - RTC module
  Wire.begin(I2C_SDA, I2C_SCL);

  #ifdef DEBUG 
    //Serial.begin(115200);
    Serial.println("Starting");
  #endif

  #ifdef DEBUG_MUSIC
    //Serial.begin(115200);
    Serial.println("Starting");
  #endif

  // Code from DFPlayerMini to initialize library
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  if (!music.begin(Serial2, true, true)) {  //Use serial to communicate with dplayer.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }

  // Init Mini Player
  music.volume(5);        // Max Volume 30

  // Start SPI
  //SPI.begin(CLK_LEFT, MISO_LEFT, DIN_LEFT, CS_LEFT);
  SPI.begin();

  // Init and Configure LED Matrix
  spiWriteRegister(DISPLAY_TEST, 0x01);  // Run test - All LED segments lit.
  delay(1000);
  spiWriteRegister(DISPLAY_TEST, 0x00);  // Finish test mode.

  spiWriteRegister(DECODE_MODE, 0x00);  // Disable BCD mode. 
  spiWriteRegister(INTENSITY, 0x01);  // Use lowest intensity.   
  spiWriteRegister(SCAN_LIMIT, 0x0F);  // Scan all digits.
  spiWriteRegister(SHUTDOWN, 0x01);  // Turn on chip.
  
  // *** Log Beginning Timestamp *** //
  // TODO: Finish adding rest of datetime data (instead of just seconds)
  uint8_t startSecs = readTimeRegister(DS3231_SECONDS);
  openLog.println(startSecs);

  // *** Randomly Select Music *** //
  // Selection of 3 songs randomly chosen from 
  // TODO: Verify that pin 15 will work for this
  randomSeed(analogRead(ANALOG_PIN));
  Serial.println(analogRead(ANALOG_PIN));
  // gFolderNumber = random(0, 3);
  // Serial.println(gFolderNumber);
  gFolderNumber = 1;

  
}

void loop() {

  // #ifdef DEBUG_MUSIC
  //   if (music.available()) {
  //     printDetail(music.readType(), music.read()); //Print the detail message from DFPlayer to handle different errors and states.
  //   }
  // #endif
  // *** Loop through all music in the folder *** //
  //music.playFolder(gFolderNumber, gSongIndex);
while (gSongIndex < 8){
  Serial.println(gSongIndex);
  music.play(gSongIndex);
  // Wait 30 seconds
  delay(29000);

  // Play next section sound effect
  //music.playFolder(gFolderNumber, NEXT_SECTION_AUDIO);
  

  // Increment song index
  gSongIndex++;
  music.stop();
  Serial.println("Stopped music");

}

  // // Check gCounter and save data to openLog
  // if(gCounter == MAX_COUNTER) {
  //   gCounter = 0;
  //   // read RTC value
  //   gSecs = readTimeRegister(DS3231_SECONDS)%60;
    
  //   // Save it to openLog
  //   openLog.print("Time Stamp: ");
  //   openLog.println(gSecs);

  //   //music.playFolder(1, 1);
  //   music.play(1);
  // }

  // // Render eyes
  // for (int i=1;i<9;i++) {
  //   spiWriteRegister(i, NUMBERS[gSecs][i-1]);
  // }

  // // increment counter
  // gCounter++;

  // // frame rate delay
  // delay(FRAME_RATE);
}
