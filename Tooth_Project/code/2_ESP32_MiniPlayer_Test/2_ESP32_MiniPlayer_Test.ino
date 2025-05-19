// https://xantorohara.github.io/led-matrix-editor
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"

// *** OpenLog Pins *** //
#define UART_TX 22
#define UART_RX 21

// *** RTC Pins *** //
#define I2C_SDA 13
#define I2C_SCL 26

// *** DF Player Pins *** //
#define RXD2 16
#define TXD2 17

#define FRAME_RATE 250  // ms
#define MAX_COUNTER 10
HardwareSerial openLog(1); // Use UART1

uint8_t gSecs = 0;
uint8_t gCounter = 0;

// *** DF Player Serial *** //
DFRobotDFPlayerMini music;

void setup() {
  //Serial.begin(9600);

  // Inti OpenLog
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

  // I2C init - RTC module
  Wire.begin(I2C_SDA, I2C_SCL);

  // Code from DFPlayerMini to initialize library
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  if (!music.begin(Serial2, true, true)) {  //Use serial to communicate with dplayer.
    //Serial.println(F("Unable to begin:"));
    //Serial.println(F("1.Please recheck the connection!"));
    //Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }

  // Init Mini Player
  music.volume(10);        // Max Volume 30

  //Serial.println(F("Ready..."));
}

void loop() {

  // Check gCounter and save data to openLog
  if(gCounter == MAX_COUNTER){
    gCounter = 0;
    // read RTC value
    gSecs = readTimeRegister(DS3231_SECONDS)%60;
    
    // Save it to openLog
    openLog.print("Time Stamp: ");
    openLog.println(gSecs);

    //music.playFolder(1, 1);
    music.play(1);
  }

  // Render eyes
  for (int i=1;i<9;i++){
    //spiWriteRegister(i, NUMBERS[gSecs][i-1]);
    //spiWriteRegister(i, data_eye_blink[i-1]);
  }

  // increment counter
  gCounter++;

  // frame rate delay
  delay(FRAME_RATE);
}
