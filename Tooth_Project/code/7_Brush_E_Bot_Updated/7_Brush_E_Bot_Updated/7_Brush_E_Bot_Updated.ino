// Eye animations
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <ANIMS.h>

// Logging and timing
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>

// Music
#include "DFRobotDFPlayerMini.h"

// ** LED Matrix Constants ** //
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2  // Two matrices
#define CS_PIN 5       // Chip Select
#define DATA_PIN 23    // MOSI
#define CLK_PIN 18     // SCK

// ** OpenLog Pins ** //
#define UART_TX 22
#define UART_RX 21

// *** RTC Pins *** //
#define I2C_SDA 13
#define I2C_SCL 26

// ** Audio Constants ** //
#define RXD2 16
#define TXD2 17
#define ANALOG_PIN 15 // For random seed

// ** Timing constants ** //
#define FRAME_RATE 5 // Frames per second
#define FRAME_DELAY 1000 / FRAME_RATE // ms


// Create display object using hardware SPI
MD_MAX72XX gEyeMatrices = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Serial interface for OpenLog
HardwareSerial openLog(1); // Use UART1
uint8_t gSectionNumber = 0; // Section number for logging

// DF Player Serial
DFRobotDFPlayerMini music;

// ** Music Variables ** //
long gFolderNumber;
uint8_t gSongIndex = 2; // Sound effect will be song /001, so music files start at /002
#define NEXT_SECTION_SFX 1
#define FINISHED_SFX 6

// DEBUGGING
void print_binary_uint8(uint8_t value) {
    Serial.print("0b");
    for (int i = 7; i >= 0; i--) {
        Serial.print((value >> i) & 1);
    }
    Serial.print("\n");
}

uint8_t reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

// ** Display a single frame of an animation on the LED matrices ** //
void displayAnimationFrame(const Anim* anim, uint8_t frame) {
  gEyeMatrices.clear();
  
  for (uint8_t row = 0; row < 8; row++) {
    // Set left matrix (device 1)
    gEyeMatrices.setRow(1, row, reverse(anim->dataLeft[frame][row]));
    //print_binary_uint8(anim->dataLeft[frame][row]);
    // Set right matrix (device 0)
    gEyeMatrices.setRow(0, row, reverse(anim->dataRight[frame][row]));
    //print_binary_uint8(anim->dataRight[frame][row]);
  }

  gEyeMatrices.update();
}

// Play an animation all the way through once
// It doesn't matter exactly how long it takes
void playAnimationOnce(const Anim* anim) {
  for (uint8_t frame = 0; frame < anim->numFrames; frame++) {
    displayAnimationFrame(anim, frame);
    delay(FRAME_DELAY);  // Frame delay
  }
}

// Play an animation all the way through once
// It should take <seconds> to play
void playAnimationInSeconds(const Anim* anim, uint8_t seconds) {
  uint16_t animDurationMS = seconds * 1000;
  uint16_t msPerFrame = animDurationMS / anim->numFrames;

  for (uint8_t frame = 0; frame < anim->numFrames; frame++) {
    displayAnimationFrame(anim, frame);
    delay(msPerFrame);  // Frame delay
  }
}

// Repeat an animation for <seconds>
void loopAnimationForSeconds(const Anim* anim, uint8_t seconds) {
  const long animStartTime = millis();
  long animCurrentTime = millis();
  long animCurrentDuration = animCurrentTime - animStartTime;

  uint16_t loopDurationMS = seconds * 1000;

  int8_t frameIncrement = 1; // 1 or -1
  uint8_t frameCounter = 0;

  while (animCurrentDuration < loopDurationMS) {

    animCurrentTime = millis();
    animCurrentDuration = animCurrentTime - animStartTime;

    displayAnimationFrame(anim, frameCounter);

    // If we got to the end, reverse
    if (frameCounter + frameIncrement >= anim->numFrames) {
      frameIncrement = -1;
    } else if (frameCounter + frameIncrement < 0) {
      frameIncrement = 1;
    }

    frameCounter += frameIncrement;

    delay(FRAME_DELAY);  // Frame delay
  }
}

// Plays sound effects
void playSoundEffect(uint8_t soundIndex) {
  music.stop(); // Stop any currently playing music
  music.playFolder(gFolderNumber, soundIndex);
  delay(1500); // Wait for SFX to finish
  music.stop();
}


void runCompleteSequence() {
  
  // TODO: Get full datetime from RTC
  openLog.print("S: ");
  Serial.println("OpenLog print");
  // TODO:
  // openLog.println(<datetime>);

  // *** WAITING SEQUENCE *** //

  // OPEN EYES + START SNIPPET 1
  Serial.println("Before Open Eyes");
  playAnimationOnce(&ANIM_OPEN_EYES);
  Serial.println("After Open Eyes");
  // TODO: Play music
  music.playFolder(gFolderNumber, gSongIndex);
  gSongIndex++; // Increment song index for next snippet

  // WAIT
  loopAnimationForSeconds(&ANIM_WAIT, 20);

  // COUNTDOWN
  playAnimationInSeconds(&ANIM_COUNTDOWN, 10);

  // SOUND EFFECT
  playSoundEffect(NEXT_SECTION_SFX);

  // *** BRUSHING SEQUENCE *** //
  
  // ** FIRST PHASE ** //
  // UPPER LEFT + START SNIPPET 2
  music.playFolder(gFolderNumber, gSongIndex);
  gSongIndex++; // Increment song index for next snippet
  loopAnimationForSeconds(&ANIM_UPPER_LEFT, 20);

  // COUNTDOWN
  playAnimationInSeconds(&ANIM_COUNTDOWN, 10);

  // TODO: Log phase 1 completion
  openLog.print(gSectionNumber++);

  // SOUND EFFECT
  playSoundEffect(NEXT_SECTION_SFX);

  // ** SECOND PHASE ** //
  
  // UPPER RIGHT + COMPLETE SFX + START SNIPPET 3
  music.playFolder(gFolderNumber, gSongIndex);
  gSongIndex++; // Increment song index for next snippet
  loopAnimationForSeconds(&ANIM_UPPER_RIGHT, 20);

  // COUNTDOWN
  playAnimationInSeconds(&ANIM_COUNTDOWN, 10);

  // LOG PHASE 2 COMPLETION
  openLog.print(gSectionNumber++);

  // SOUND EFFECT
  playSoundEffect(NEXT_SECTION_SFX);

  // ** THIRD PHASE ** //
  
  // LOWER LEFT + COMPLETE SFX + START SNIPPET 4
  music.playFolder(gFolderNumber, gSongIndex);
  gSongIndex++; // Increment song index for next snippet
  loopAnimationForSeconds(&ANIM_LOWER_LEFT, 20);

  // COUNTDOWN
  playAnimationInSeconds(&ANIM_COUNTDOWN, 10);

  // LOG PHASE 3 COMPLETION
  openLog.print(gSectionNumber++);

  // SOUND EFFECT
  playSoundEffect(NEXT_SECTION_SFX);

  // ** FOURTH PHASE ** //

  // LOWER RIGHT + COMPLETE SFX + START SNIPPET 5
  music.playFolder(gFolderNumber, gSongIndex);
  gSongIndex++; // Increment song index for next snippet
  loopAnimationForSeconds(&ANIM_LOWER_RIGHT, 20);

  // COUNTDOWN
  playAnimationInSeconds(&ANIM_COUNTDOWN, 10);

  // EXCITED + FINAL SFX
  playSoundEffect(FINISHED_SFX);
  loopAnimationForSeconds(&ANIM_EXCITED_EYES, 10);
}


void setup() {
  // Serial monitor output
  Serial.begin(9600);

  // Init eye matrices
  Serial.println("Begin eyes");
  gEyeMatrices.begin();
  gEyeMatrices.control(MD_MAX72XX::INTENSITY, 5);
  gEyeMatrices.clear();

  // Init OpenLog
  Serial.println("OpenLog");
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

  // Init RTC
  Serial.println("RTC");
  Wire.begin(I2C_SDA, I2C_SCL);

  // Code from DFPlayerMini to initialize library
  Serial.println("DF Before If");
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  if (!music.begin(Serial2, true, true)) {  //Use serial to communicate with dplayer.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println("DF After If");

    // Init Mini Player
  randomSeed(analogRead(ANALOG_PIN));
  gFolderNumber = random(0, 3);

  // Run the complete sequence
  Serial.println("Begin Comp Sequence");
  runCompleteSequence();
}

void loop() {
  // Play wait animation until the robot is shut off
  playAnimationOnce(&ANIM_WAIT);
}
