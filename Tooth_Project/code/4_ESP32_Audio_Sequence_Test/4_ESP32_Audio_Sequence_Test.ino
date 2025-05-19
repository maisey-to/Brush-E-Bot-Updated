// https://xantorohara.github.io/led-matrix-editor
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include "MAX7219.h"
#include "SETUP.h"

// *** Use UART1 for RTC *** //
HardwareSerial openLog(1); 

// *** DF Player Serial *** //
DFRobotDFPlayerMini music;

// *** Globals *** //
#define FRAME_RATE 250  // ms
#define SONG_DURATION 30000 // ms
#define NEXT_SFX 2000 // ms
#define FINISHED_SFX 4000 // ms

// ** Sequence Variables ** //
uint8_t gCurrentPhase = 0; // There will be 5 phases
int gPhaseDuration = 0;
long gCurrentPhaseElapsed = 0;

// ** Music Variables ** //
long gFolderNumber;
uint8_t gSongIndex = 2; // Sound effect will be song /001, so music files start at /002
uint8_t NEXT_SECTION_AUDIO = 1;

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
  music.volume(10);        // Max Volume 30

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
  delay(FRAME_RATE);
  // #ifdef DEBUG_MUSIC
  //   if (music.available()) {
  //     printDetail(music.readType(), music.read()); //Print the detail message from DFPlayer to handle different errors and states.
  //   }
  // #endif
  // *** Loop through all music in the folder *** //
  //music.playFolder(gFolderNumber, gSongIndex);
  while (gSongIndex < 7){
    delay(250);

    Serial.println("Playing audio " + gSongIndex);
    //music.play(gSongIndex);
    music.playFolder(gFolderNumber, gSongIndex);
    // Wait 30 seconds
    delay(29000);
    music.stop();
    Serial.println("Section complete ");

    // Play next section sound effect
    music.playFolder(gFolderNumber, 1);

    // Increment song index
    
    Serial.println("Sound effect complete");
    gSongIndex++;
    delay(1000);
  }
}
