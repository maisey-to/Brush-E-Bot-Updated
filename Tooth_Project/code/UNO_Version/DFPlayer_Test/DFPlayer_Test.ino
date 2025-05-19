
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

// *** DF Player Serial *** //
DFRobotDFPlayerMini music;
SoftwareSerial miniPlayerSerial(4, 5); // RX and TX

void setup()   {

  // Init Mini Player
  miniPlayerSerial.begin(9600);

  if (!music.begin(miniPlayerSerial, true, true)) {
    //Serial.println(F("Not initialized:"));
    //Serial.println(F("1. Check the DFPlayer Mini connections"));
    //Serial.println(F("2. Insert an SD card"));
    while (true);
  }

  // Init Mini Player
  //music.setTimeOut(500);  // Serial timeout 500ms
  music.volume(10);        // Max Volume 30
  //music.EQ(0);            // Normal equalization
  //music.playFolder(1, 1);

}

void loop()  {

  //music.playFolder(1, 1);
  //music.start();
  music.play(1);

  // frame rate delay
  delay(10000);
  music.stop();
  delay(2000);
}


