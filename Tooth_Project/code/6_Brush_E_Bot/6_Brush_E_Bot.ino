#include <MD_MAX72XX.h>
#include <SPI.h>
#include <ANIMS.h>

// -- LED Matrix Constants -- //
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2  // Two matrices
#define CS_PIN 5       // Chip Select
#define DATA_PIN 23    // MOSI
#define CLK_PIN 18     // SCK

// Timing constants
#define FRAME_RATE 5 // Frames per second
const uint8_t gFramePeriod = 1000 / FRAME_RATE // ms

enum STATE
{
  OPEN_EYES,
  WAIT,
  COUNTDOWN,
  UPPER_LEFT,
  UPPER_RIGHT,
  LOWER_LEFT,
  LOWER_RIGHT,
  EXCITED
};

const STATE gSequence[13] = {
  STATE.OPEN_EYES,
  STATE.WAIT,
  STATE.
};

// Animation variables
uint8_t gCurrentAnimDuration;
const Anim* gCurrentAnimLeft;
const Anim* gCurrentAnimRight;

// Const array 
const Anim* const gAnimList[13] = {

}

// Create display object using hardware SPI
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);


void runCompleteSequence() {
  // OPEN EYES + START SNIPPET 1

  for (uint8_t i = 0; i < ANIM_OPEN_EYES.num_frames; i++)
  {
    mx.clear();
    
    for (int row = 0; row < 8; row++) {
      // Set left matrix (device 1)
      mx.setRow(1, row, ANIM_OPEN_EYES.data_left[frame][row]);
      mx.setRow(0, row, ANIM_OPEN_EYES.data_right[frame][row]);
    }

    mx.update();
    delay(150);  // Frame delay
  }

  // WAIT
  while 

  // COUNTDOWN
  // UPPER LEFT + START SNIPPET 2
  // COUNTDOWN
  // UPPER RIGHT + COMPLETE SFX + START SNIPPET 3
  // COUNTDOWN
  // LOWER LEFT + COMPLETE SFX + START SNIPPET 4
  // COUNTDOWN
  // LOWER RIGHT + COMPLETE SFX + START SNIPPET 5
  // COUNTDOWN
  // EXCITED + FINAL SFX
  // CLOSE EYES
  // STOP MUSIC

}


void setup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 5);
  mx.clear();
}

void loop() {

  const currentMillis = millis();

  // Handle switching anim

  // Handle switching song

  // Handle switching frame


  for (int frame = 0; frame < 8; frame++) {
    mx.clear();
    
    for (int row = 0; row < 8; row++) {
      // Set left matrix (device 1)
      mx.setRow(1, row, DATA_EYE_BLINK[frame][row]);

      // Optionally clear right matrix (device 0)
      mx.setRow(0, row, 0x00);
    }

    mx.update();
    delay(150);  // Frame delay
  }

  delay(500);  // Pause before repeating animation
}
