#include <Wire.h>
#include <DS3231-RTC.h>

// *** RTC Pins *** //
#define I2C_SDA 13
#define I2C_SCL 26

// RTC Library
RTClib rtc;
DS3231 Clock;

constexpr time_t tstmp {1750625450UL};

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  // Init RTC
  Serial.println("RTC");
  Wire.begin(I2C_SDA, I2C_SCL);

  delay(2000);

  // feed UnixTimeStamp and don' t use localtime
  Clock.setEpoch(tstmp, false);
  // set to 24h
  Clock.setClockMode(false);

  // Just for verification of DS3231 Data
  // check now the data from ESP8266 and DS3231
  // get year
  bool century = false;
  bool h12Flag;
  bool pmFlag;

  // read directly from DS3231 module
  Serial.print("\n\n");
  Serial.print(" DateTime of DS3231:     ");
  Serial.print(Clock.getYear(), DEC);
  Serial.print("-");
  Serial.print(Clock.getMonth(century), DEC);
  Serial.print("-");
  Serial.print(Clock.getDate(), DEC);
  Serial.print(" ");
  Serial.print(Clock.getHour(h12Flag, pmFlag), DEC);
  Serial.print(":");
  Serial.print(Clock.getMinute(), DEC);
  Serial.print(":");
  Serial.print(Clock.getSecond(), DEC);
  Serial.print("  -  weekday ");
  Serial.print(Clock.getDoW(), DEC);
  Serial.println();

  // Read now from DateTime class
  DateTime datetime = rtc.now();
  Serial.print("\n\nData of Struct tm\n");
  Serial.print(" DateTime of RTC:        ");
  Serial.print(datetime.getYear(), DEC);
  Serial.print("-");
  Serial.print(datetime.getMonth(), DEC);
  Serial.print("-");
  Serial.print(datetime.getDay(), DEC);
  Serial.print(" ");
  Serial.print(datetime.getHour(), DEC);
  Serial.print(":");
  Serial.print(datetime.getMinute(), DEC);
  Serial.print(":");
  Serial.print(datetime.getSecond(), DEC);
  Serial.print("  -  weekday ");
  Serial.print(datetime.getWeekDay(), DEC);
  Serial.println();
  Serial.print(" Unixtime: ");
  Serial.println(datetime.getUnixTime());
  Serial.print(" Y2k-Time: ");
  Serial.println(datetime.getY2kTime());
}

void loop() {
  // put your main code here, to run repeatedly:

}
