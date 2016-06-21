/*
 * VEML6075.cpp
 *
 * Arduino library for the Vishay VEML6075 UVA/UVB i2c sensor.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 */

#include "VEML6075.h"

VEML6075::VEML6075() {
}

void VEML6075::begin() {
  Wire.begin();
  this->write16(VEML6075_REG_CONF, VEML6075_CONF_IT_50MS
    | VEML6075_CONF_HD_NORM
    | VEML6075_CONF_AF_OFF
    | VEML6075_CONF_SD_OFF
  );
  Wire.endTransmission();
}

uint16_t VEML6075::getUVA() {
  return this->read16(VEML6075_REG_UVA);
}

uint16_t VEML6075::getUVB() {
  return this->read16(VEML6075_REG_UVB);
}

uint16_t VEML6075::getUVDummy() {
  return this->read16(VEML6075_REG_DUMMY);
}

uint16_t VEML6075::getUVcomp1() {
  return this->read16(VEML6075_REG_UVCOMP1);
}

uint16_t VEML6075::getUVcomp2() {
  return this->read16(VEML6075_REG_UVCOMP2);
}

uint16_t VEML6075::getDevID() {
  return this->read16(VEML6075_REG_DEVID);
}

uint16_t VEML6075::read16(uint8_t reg) {
  uint8_t msb = 0;
  uint8_t lsb = 0;

  Wire.beginTransmission(VEML6075_ADDR);
  Wire.write(reg);
  Wire.endTransmission();

  Wire.requestFrom(VEML6075_ADDR, 2);
  lsb = Wire.read();
  msb = Wire.read();

  return (msb << 8) | lsb;
}

void VEML6075::write16(uint8_t reg, uint16_t data) {
  Wire.beginTransmission(VEML6075_ADDR);
  Wire.write(reg);
  Wire.write((uint8_t)(0xFF & (data >> 0))); // LSB
  Wire.write((uint8_t)(0xFF & (data >> 8))); // MSB
  Wire.endTransmission();
}
