#include <Arduino.h>
#include <LedControl.h>
#include "DFRobotDFPlayerMini.h"
#include "anims.h"

// Uncomment this to get debug info in the serial monitor
// #define DEBUG
#define DEBUG_MUSIC

// *** Define ESP Pins *** //

// *** Eye LedControl pins *** //
#define DIN_LEFT 23
#define CS_LEFT 5
#define CLK_LEFT 18

// *** DF Player Pins *** //
#define RXD2 16
#define TXD2 17

long start_time = 0;

// *** Eye LedControl objects *** //
// LedControl(DataIn, Clock, ChipSelect, NumberOfLEDMatrices)
LedControl eye_leds = LedControl(DIN_LEFT, CLK_LEFT, CS_LEFT, 2);

// *** DF Player Serial *** //
DFRobotDFPlayerMini music;

// Keep track of which frame we're at in the animation
int frame_counter = 0;

// Pointers to the current animation
const Anim *current_anim_left;
const Anim *current_anim_right;

// Track different pieces of information about each animation
int current_anim_duration = 0;
int current_anim_num_frames = 0;
long current_anim_start_time = 0;

const int NUM_PHASES = 13;

// All of the animations, in pairs of two, one for each eye
const Anim *const ANIM_LIST[26] = {
    &ANIM_OPEN_EYES,
    &ANIM_OPEN_EYES,
    &ANIM_WAIT_LEFT,
    &ANIM_WAIT_RIGHT,
    &ANIM_COUNTDOWN,
    &ANIM_COUNTDOWN,
    &ANIM_UPPER_LEFT_LEFT,
    &ANIM_UPPER_LEFT_RIGHT,
    &ANIM_COUNTDOWN,
    &ANIM_COUNTDOWN,
    &ANIM_UPPER_RIGHT_LEFT,
    &ANIM_UPPER_RIGHT_RIGHT,
    &ANIM_COUNTDOWN,
    &ANIM_COUNTDOWN,
    &ANIM_LOWER_LEFT_LEFT,
    &ANIM_LOWER_LEFT_RIGHT,
    &ANIM_COUNTDOWN,
    &ANIM_COUNTDOWN,
    &ANIM_LOWER_RIGHT_LEFT,
    &ANIM_LOWER_RIGHT_RIGHT,
    &ANIM_COUNTDOWN,
    &ANIM_COUNTDOWN,
    &ANIM_EXCITED_EYES,
    &ANIM_EXCITED_EYES,
    &ANIM_CLOSE_EYES,
    &ANIM_CLOSE_EYES,};

// The duration that each animation should play for
//   0: play once
//   > 0: play for x number of seconds
//   < 0: take x seconds to play once
//   Differentiate between animations that need to play in a 
//   certain amount of time (countdown) vs just playing for a certain amount of time (eye movement)
int ANIM_DURATION[13] = {
    0,
    10,
    -10,
    20,
    -10,
    20,
    -10,
    20,
    -10,
    20,
    -10,
    10,
    0
};

// Number of frames in each animation in the sequence
int anim_frame_count[13] = {
    7,
    8,
    40,
    12,
    40,
    12,
    40,
    13,
    40,
    13,
    40,
    8,
    7
};

// Track the status of each animation phase
//    0 = not complete
//    1 = complete
int phase_complete[13] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};

// Current phase
int phase = 0;
// Current animation (2 per phase)
int anim_idx = 0;

// Did we just start a new phase?
bool is_new_phase = false;

// Did we just start running?
bool just_started = false;

// Value to increment the frame counter by
// For some animations, it will become -1 to play in reverse
int frame_step = 1;

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

// Setup runs once when the microcontroller first turns on
void setup()
{
    start_time = millis();

    // Initialize the LED matrices
    // Loop through each matrix (2 of them)
    for (int i = 0; i < eye_leds.getDeviceCount(); i++)
    {
        // Initialize them to be off
        eye_leds.shutdown(i, false);
        eye_leds.setIntensity(i, 0);
        eye_leds.clearDisplay(i);
    }

#ifdef DEBUG 
    Serial.begin(115200);
    Serial.println("Starting");
#endif

#ifdef DEBUG_MUSIC
    Serial.begin(115200);
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
    Serial.println(F("DFPlayer Mini online."));

    // Initialize values to first animation
    current_anim_duration = ANIM_DURATION[0];
    current_anim_left = ANIM_LIST[0];
    current_anim_right = ANIM_LIST[1];
    current_anim_start_time = millis();
    current_anim_num_frames = anim_frame_count[0];
}

// Loop runs over and over again throught
void loop()
{

#ifdef DEBUG_MUSIC
    if (music.available()) {
      printDetail(music.readType(), music.read()); //Print the detail message from DFPlayer to handle different errors and states.
    }
#endif

    // Delay to keep animations from playing as fast as possible
    delay(250);

    // if phase is complete, move to next phase
    if (phase_complete[phase])
    {
        phase++;
        anim_idx += 2;

        if (phase == 0) // First phase
        {
            // Play intro music
            music.volume(10);
            music.playFolder(1, 1);
        }
        else if (phase == 3) // First brushing phase
        {
            music.stop();
            // Play a random song from the three options
            music.playFolder(1, random(2, 4));
        }

        // If that was the last phase, reset everything
        if (phase >= NUM_PHASES)
        {
            phase = 0;
            anim_idx = 0;

            for (int i = 0; i < NUM_PHASES; i++)
            {
                phase_complete[i] = 0;
            }

            // Play end sound
            music.stop();
            music.volume(10);
            music.playFolder(1, 5);

            delay(1000);

            // Clear eyes
            eye_leds.clearDisplay(0);
            eye_leds.clearDisplay(1);

            // Go to sleep
            esp_deep_sleep_start();

            return;
        }

        is_new_phase = true;

#ifdef DEBUG
        Serial.println("Starting phase " + String(phase));
#endif
    }

    // If we just started a new phase, set values for next animation
    if (is_new_phase)
    {
        is_new_phase = false;

        // Set up variables for this phase
        current_anim_duration = ANIM_DURATION[phase];
        current_anim_left = ANIM_LIST[anim_idx];
        current_anim_right = ANIM_LIST[anim_idx + 1];
        current_anim_start_time = millis();
        current_anim_num_frames = anim_frame_count[phase];

        frame_counter = 0;
    }

    if (current_anim_duration == 0)
    {
        // The anim should be played only once.
#ifdef DEBUG
        Serial.println("Frame counter: " + String(frame_counter) + " / " + String(current_anim_num_frames));
#endif
        // It's an 8x8 matrix, so loop through 8 rows and set all the 8 columns in that row
        //   Do this for both eyes (the first parameter of setRow specifies which eye)
        for (int i = 0; i < 8; i++)
        {
            eye_leds.setRow(0, i, current_anim_left->anim[(frame_counter * 8) + i]);
            eye_leds.setRow(1, i, current_anim_right->anim[(frame_counter * 8) + i]);
        }

        // Increment frame_counter if we haven't made it through the animation
        if (frame_counter < current_anim_num_frames - 1)
        {
            frame_counter++;
        }
        else // The animation is done
        {
            phase_complete[phase] = 1;
        }
    }
    else if (current_anim_duration > 0)
    {
        // The anim should be looped for a certain duration

        long current_time = millis();

        // Have we been running for longer than the duration? If so, done.
        if (current_time - current_anim_start_time > current_anim_duration * 1000)
        {
            phase_complete[phase] = 1;
        }
        else
        {
#ifdef DEBUG
            Serial.println("Frame counter: " + String(frame_counter) + " / " + String(current_anim_num_frames));

            // Print a message about how long we have been looping
            Serial.println("Current time: " + String(current_time - current_anim_start_time) + " / " + String(current_anim_duration * 1000));
#endif
            for (int i = 0; i < 8; i++)
            {
                eye_leds.setRow(0, i, current_anim_left->anim[(frame_counter * 8) + i]);
                eye_leds.setRow(1, i, current_anim_right->anim[(frame_counter * 8) + i]);
            }

            // When we reach the end of the animation, reverse the direction that it's iterating
            //   and play the animation backwards
            if (frame_counter == current_anim_num_frames - 1)
            {
                frame_step = -1;
            }
            else if (frame_counter == 0)
            {
                frame_step = 1;
            }

            frame_counter += frame_step;
        }
    }
    else
    {
        // The anim should take a certain duration to play once

        long current_time = millis();

        if (current_time - current_anim_start_time > (-current_anim_duration * 1000))
        {
            phase_complete[phase] = 1;
        }
        else
        {
#ifdef DEBUG
            Serial.println("Frame counter: " + String(frame_counter) + " / " + String(current_anim_num_frames));
#endif
            for (int i = 0; i < 8; i++)
            {
                eye_leds.setRow(0, i, current_anim_left->anim[(frame_counter * 8) + i]);
                eye_leds.setRow(1, i, current_anim_right->anim[(frame_counter * 8) + i]);
            }

            // Sleep for a duration to ensure the whole animation takes the right amount of time
            // Animation total duration / number of frames = duration per frame
            // Subtract 250 for the main loop delay
            // This doesn't account for the time it takes for the code to actually run, but it's close enough
            delay(((-current_anim_duration) * 1000 / current_anim_num_frames) - 250);

            if (frame_counter < current_anim_num_frames - 1)
            {
                frame_counter++;
            }
            else
            {
                frame_counter = 0;
            }
        }
    }
}

