//MAX7219 - using SPI library to display 8x8 bitmap
#include <SPI.h>

// SPI Pins
#define CS 10

// MAX7219 Control registers
#define DECODE_MODE     0x09 
#define INTENSITY       0x0A
#define SCAN_LIMIT      0x0B 
#define SHUTDOWN        0x0C
#define DISPLAY_TEST    0x0F

const uint8_t Apple[8] = {B00011000,B00001000,B01110110,B11111111,B11111111,B11111111,B01111010,B00110100};
const uint8_t Cat[8] = {B10001000,B11111000,B10101000,B01110001,B00100001,B01111001,B01111101,B10111110};

const uint8_t IMAGES[][8] = {
  {0b00000111,0b00000000,0b00000101,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000},
  {0b00000111,0b00000000,0b00000101,0b00000010,0b00000010,0b00001110,0b00000000,0b00000000},
  {0b00000111,0b00000000,0b00000101,0b00100010,0b00100010,0b00111110,0b00000000,0b00000000},
  {0b00000111,0b00111000,0b00100101,0b00100010,0b00100010,0b00111110,0b00000000,0b00000000},
  {0b00000000,0b00000000,0b00110110,0b00011100,0b01111111,0b00011100,0b00110110,0b00000000},
  {0b00000000,0b00100010,0b01110111,0b01111111,0b01111111,0b00111110,0b00011100,0b00001000}
};
const int IMAGES_LEN = sizeof(IMAGES)/8;

void setup()   {
  pinMode(CS, OUTPUT);

  SPI.begin();  // Start SPI
  
  writeRegister(DISPLAY_TEST, 0x01);  // Run test - All LED segments lit.
  delay(1000);
  writeRegister(DISPLAY_TEST, 0x00);  // Finish test mode.

  writeRegister(DECODE_MODE, 0x00);  // Disable BCD mode. 
  writeRegister(INTENSITY, 0x01);  // Use lowest intensity.   
  writeRegister(SCAN_LIMIT, 0x0F);  // Scan all digits.
  writeRegister(SHUTDOWN, 0x01);  // Turn on chip.
}

void loop()  {
  //displayIndividualImg();
  displaySequence();
}

void displayIndividualImg(){
  // Display Apple
  for (int i=1;i<9;i++)   
    writeRegister(i, Apple[i-1]);
  // Wait 3secs
  delay(3000);
  // Display Cat
  for (int i=1;i<9;i++)   
    writeRegister(i, Cat[i-1]);
  // Wait 3secs
  delay(3000);
}

void displaySequence(){
  // https://xantorohara.github.io/led-matrix-editor/#

  for (int j=0;j<IMAGES_LEN; j++){
    for (int i=1;i<9;i++)   
      writeRegister(i, IMAGES[j][i-1]);
    delay(300);
  }
}

void writeRegister(uint8_t reg, uint8_t value) { 
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)); 
  digitalWrite(CS, LOW);  // start transfer  
  SPI.transfer(reg);  // Send address.
  SPI.transfer(value);  // Send the value.
  digitalWrite(CS, HIGH);  // Finish transfer.
  SPI.endTransaction();
}