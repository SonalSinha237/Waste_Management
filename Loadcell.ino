#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
void setup() {
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
  if (scale.is_ready()) {
    long reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }
delay(1000);
}

/*
 void loop() {
 
  if (scale.is_ready()) {
    long reading = scale.read() * 266;
    float grams = (float)reading / 100000.0;
    grams = grams - 996.0;
    Serial.print("HX711 reading: ");
    Serial.print(grams);
    Serial.println(" g");
  } else {
    Serial.println("HX711 not found.");
  }
 
  delay(1000);
  
}
 */
