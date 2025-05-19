#include <MD_MAX72XX.h>
#include <SPI.h>

// Define hardware type and connections
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW
#define MAX_DEVICES 2  // Two matrices
#define CS_PIN 5        // Chip Select
#define DATA_PIN 23     // MOSI
#define CLK_PIN 18      // SCK

// Create display object using hardware SPI
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Eye blink animation frames
const byte DATA_EYE_BLINK[8][8] = {
  {0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b00000000, 0b00111100, 0b01000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b00011000, 0b00100100, 0b01000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b00000000, 0b00111100, 0b01000010, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b00000000, 0b00000000, 0b01111110, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b00000000, 0b00000000, 0b01000010, 0b00111100, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b00000000, 0b00000000, 0b01000010, 0b00100100, 0b00011000, 0b00000000, 0b00000000},
  {0b00000000, 0b00000000, 0b00000000, 0b01000010, 0b00111100, 0b00000000, 0b00000000, 0b00000000}
};

void setup() {
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 5);
  mx.clear();
}

void loop() {
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
