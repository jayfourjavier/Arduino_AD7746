#include <Arduino.h>
#include <Wire.h>
#include <Arduino_AD7746.h>

Arduino_AD7746 cdc;

void setup() {
  Serial.begin(9600);
  Wire.begin();           // 100 kHz is fine; bump later if desired

  if (!cdc.testConnection()) {
    Serial.println("AD7746 not found at 0x48. Check wiring/pull-ups.");
    while (1) delay(1000);
  }

  if (!cdc.begin()) {
    Serial.println("AD7746 init failed.");
    while (1) delay(1000);
  }

  Serial.println("AD7746 ready. Printing AVG(20) every second...");
}

void loop() {
  double avg = cdc.readAverage(20);
  Serial.print("AVG(20): ");
  Serial.print(avg, 6);
  Serial.println(" pF");
  delay(1000);
}
