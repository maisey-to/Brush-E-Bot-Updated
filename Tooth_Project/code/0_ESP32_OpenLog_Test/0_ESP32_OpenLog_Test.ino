#include <HardwareSerial.h>

#define UART_TX 22
#define UART_RX 21

HardwareSerial openLog(1); // Use UART1

uint8_t gCounter = 0;

void setup() {
  // Inti OpenLog
  openLog.begin(9600, SERIAL_8N1, UART_RX, UART_TX);
  delay(1000);

}

void loop() {
  gCounter++;
  openLog.println(gCounter);
  delay(1000);
}
