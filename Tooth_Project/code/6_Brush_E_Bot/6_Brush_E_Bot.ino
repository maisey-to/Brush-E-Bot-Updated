// Eye animations
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <ANIMS.h>

// Logging and timing
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>

// -- LED Matrix Constants -- //
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2  // Two matrices
#define CS_PIN 5       // Chip Select
#define DATA_PIN 23    // MOSI
#define CLK_PIN 18     // SCK

// -- Logging and Timing Constants -- //
#define UART_TX 22
#define UART_RX 21

#define I2C_SDA 4
#define I2C_SCL 5

// -- Audio Constants -- //
#define RXD2 16
#define TXD2 17

// -- Timing constants -- //
#define FRAME_RATE 5 // Frames per second
#define FRAME_DELAY = 1000 / FRAME_RATE // ms


// Create display object using hardware SPI
MD_MAX72XX gEyeMatrices = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Serial interface for OpenLog
HardwareSerial openLog(1); // Use UART1

// DF Player Serial
DFRobotDFPlayerMini music;


// Display a single frame of an animation on the LED matrices
void displayAnimationFrame(const Anim* anim, uint8_t frame) {
  gEyeMatrices.clear();
  
  for (uint8_t row = 0; row < 8; row++) {
    // Set left matrix (device 1)
    gEyeMatrices.setRow(1, row, anim->dataLeft[frame][row]);
    // Set right matrix (device 0)
    gEyeMatrices.setRow(0, row, anim->dataRight[frame][row]);
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

void runCompleteSequence() {
  // TODO: Get full datetime from RTC
  openLog.print("S: ");
  // TODO:
  // openLog.println(<datetime>);

  // OPEN EYES + START SNIPPET 1
  playAnimationOnce(ANIM_OPEN_EYES);
  // TODO: Play music

  // WAIT
  loopAnimationForSeconds(ANIM_WAIT, 10);

  // COUNTDOWN
  playAnimationInSeconds(ANIM_COUNTDOWN, 10);

  // UPPER LEFT + START SNIPPET 2
  loopAnimationForSeconds(ANIM_UPPER_LEFT, 20);
  // TODO: Play music snippet 2

  // COUNTDOWN
  playAnimationInSeconds(ANIM_COUNTDOWN, 10);

  // TODO: Log phase 1 completion

  // UPPER RIGHT + COMPLETE SFX + START SNIPPET 3
  loopAnimationForSeconds(ANIM_UPPER_RIGHT, 20);
  // TODO: Play music snippet 3

  // COUNTDOWN
  playAnimationInSeconds(ANIM_COUNTDOWN, 10);

  // LOWER LEFT + COMPLETE SFX + START SNIPPET 4
  loopAnimationForSeconds(ANIM_LOWER_LEFT, 20);
  // TODO: Play music snippet 4

  // COUNTDOWN
  playAnimationInSeconds(ANIM_COUNTDOWN, 10);

  // LOWER RIGHT + COMPLETE SFX + START SNIPPET 5
  loopAnimationForSeconds(ANIM_LOWER_RIGHT, 20);
  // TODO: Play music snippet 5

  // COUNTDOWN
  playAnimationInSeconds(ANIM_COUNTDOWN, 10);

  // EXCITED + FINAL SFX
  // TODO: Play final SFX
  loopAnimationForSeconds(ANIM_EXCITED_EYES, 10);

  // TODO: Decide if this is what we want
  // CLOSE EYES
  // STOP MUSIC

}


void setup() {
  // Init eye matrices
  gEyeMatrices.begin();
  gEyeMatrices.control(MD_MAX72XX::INTENSITY, 5);
  gEyeMatrices.clear();

  // Init OpenLog
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

  // Init RTC
  Wire.begin(I2C_SDA, I2C_SCL);

  // Run the complete sequence
  runCompleteSequence();
}

void loop() {

  playAnimationOnce(ANIM_WAIT);
}
