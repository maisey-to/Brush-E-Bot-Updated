#include <SoftwareSerial.h>

SoftwareSerial openLog(8, 9);  // RX, TX

void setup() {
  Serial.begin(9600);
  openLog.begin(9600);

  delay(1000);  // Give OpenLog time to initialize

  openLog.println("Logging started...");
}

void loop() {
  int sensorValue = analogRead(A0);
  
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);
  
  openLog.print("Sensor Value: ");
  openLog.println(sensorValue);

  delay(1000);  // Log every second

}
