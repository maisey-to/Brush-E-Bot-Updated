// https://xantorohara.github.io/led-matrix-editor
#include "DS3231.h"
#include <HardwareSerial.h>
#include <Wire.h>

#define UART_TX 22
#define UART_RX 21

#define I2C_SDA 4
#define I2C_SCL 5

#define FRAME_RATE 250  // ms
#define MAX_COUNTER 10
HardwareSerial openLog(1); // Use UART1

uint8_t gSecs;
uint8_t gCounter = 0;

void setup() {
  // Inti OpenLog
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

  // I2C init - RTC module
  Wire.begin(I2C_SDA, I2C_SCL);

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
    //music.play(1);
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
