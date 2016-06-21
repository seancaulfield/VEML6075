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

void setup() {
  Serial.begin(9600);
}

void loop() {
  uint16_t value;

  value = veml6075.getUVA();
  Serial.print(F("UVA = "));
  Serial.println(value);

  value = veml6075.getUVB();
  Serial.print(F("UVB = "));
  Serial.println(value);

  value = veml6075.getUVDummy();
  Serial.print(F("UVDummy = "));
  Serial.println(value);

  value = veml6075.getUVcomp1();
  Serial.print(F("UV comp 1 = "));
  Serial.println(value);

  value = veml6075.getUVcomp2();
  Serial.print(F("UV comp 2 = "));
  Serial.println(value);

  value = veml6075.getDevID();
  Serial.print(F("Device ID = "));
  Serial.println(value);

  delay(1000);
}
