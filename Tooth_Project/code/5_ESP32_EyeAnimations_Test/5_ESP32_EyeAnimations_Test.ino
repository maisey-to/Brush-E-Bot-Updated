// https://xantorohara.github.io/led-matrix-editor
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include "ANIMS.h"
#include <MD_MAX72xx.h>

// *** OpenLog Pins *** //
#define UART_TX 22
#define UART_RX 21

// *** RTC Pins *** //
#define I2C_SDA 13
#define I2C_SCL 26

// *** DF Player Pins *** //
#define RXD2 16
#define TXD2 17

// *** Eye LedControl devices and pins *** //
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2

#define DATA_PIN 23
#define MISO_PIN 19
#define CS_PIN 5
#define CLK_PIN 18

// SPI hardware interface
MD_MAX72XX M = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// *** Global Variables *** //
#define FRAME_RATE 250  // ms
HardwareSerial openLog(1); // Use UART1

uint8_t gSecs = 0;
uint8_t gCounter = 0;

uint8_t gCurrentFrame = 0;

// *** DF Player Serial *** //
DFRobotDFPlayerMini music;
void setup() {
  Serial.begin(9600);

  // Inti OpenLog
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

  // I2C init - RTC module
  Wire.begin(I2C_SDA, I2C_SCL);

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
  music.volume(10);        // Max Volume 30


  // Older LED code
  // Start SPI
  //SPI.begin(CLK_LEFT, MISO_LEFT, DIN_LEFT, CS_LEFT);
  // SPI.begin();

  // Init and Configure LED Matrix
  // spiWriteRegister(DISPLAY_TEST, 0x01);  // Run test - All LED segments lit.
  // delay(1000);
  // spiWriteRegister(DISPLAY_TEST, 0x00);  // Finish test mode.

  // spiWriteRegister(DECODE_MODE, 0x00);  // Disable BCD mode. 
  // spiWriteRegister(INTENSITY, 0x01);  // Use lowest intensity.   
  // spiWriteRegister(SCAN_LIMIT, 0x0F);  // Scan all digits.
  // spiWriteRegister(SHUTDOWN, 0x01);  // Turn on chip.
}

void loop() {

  if (gCurrentFrame == ANIM_EYE_BLINK.num_frames) {
    // We're done with this animation

    return; // Just return for testing
  }

  
  // For the current frame, loop through and set
  for (int i=0; i<8; i++) {
    M.setRow(0, i, ANIM_EYE_BLINK.anim[gCurrentFrame][i]);
    M.setRow(1, i, ANIM_EYE_BLINK.anim[gCurrentFrame][i]);
  }

  // increment counter
  gCurrentFrame++;

  // frame rate delay
  delay(FRAME_RATE);
}
