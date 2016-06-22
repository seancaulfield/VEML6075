/*
 * test-veml6075.ino
 *
 * Simpel sketch to test if a given board words.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 */

#include <Arduino.h>
#include <Wire.h>
#include <VEML6075.h>

VEML6075 veml6075 = VEML6075();
bool found = false;

void setup() {
  delay(1000);
  Serial.begin(57600);
  Wire.begin();
  if (!veml6075.begin()) {
    Serial.println(F("VEML6075 not found!"));
  } else {
    found = true;
  }
}

void loop() {

  if (found) {
    uint16_t value;

    Serial.print(F("t = "));
    Serial.println(millis());

    value = veml6075.getUVA();
    Serial.print(F("UVA = "));
    Serial.println(value);

    value = veml6075.getUVB();
    Serial.print(F("UVB = "));
    Serial.println(value);

    value = veml6075.getUVIndex();
    Serial.print(F("UV Index = "));
    Serial.println(value);

    value = veml6075.getDevID();
    Serial.print(F("Device ID = "));
    Serial.println(value, HEX);

    Serial.println(F("----------------"));
  }

    delay(1000);
}
