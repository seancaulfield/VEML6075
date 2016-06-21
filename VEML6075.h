/*
 * VEML6075.h
 *
 * Arduino library for the Vishay VEML6075 UVA/UVB i2c sensor.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * License: GPLv2.0
 *
 */

#ifndef _VEML6075_H
#define _VEML6075_H

#define VEML6075_ADDR 0x10

#define VEML6075_REG_CONF        (0x00) // Configuration register (options below)
#define VEML6075_REG_UVA         (0x07) // UVA register
#define VEML6075_REG_DUMMY       (0x08) // Dummy register
#define VEML6075_REG_UVB         (0x09) // UVB register
#define VEML6075_REG_UVCOMP1     (0x0A) // UV comp1 register
#define VEML6075_REG_UVCOMP2     (0x0B) // UV comp2 register
#define VEML6075_REG_DEVID       (0x0C) // Device ID register

#define VEML6075_CONF_IT_50MS    (0x00) // Integration time = 50ms (default)
#define VEML6075_CONF_IT_100MS   (0x10) // Integration time = 100ms
#define VEML6075_CONF_IT_200MS   (0x20) // Integration time = 200ms
#define VEML6075_CONF_IT_400MS   (0x30) // Integration time = 400ms
#define VEML6075_CONF_IT_800MS   (0x40) // Integration time = 800ms

#define VEML6075_CONF_HD_NORM    (0x00) // Normal dynamic seetting (default)
#define VEML6075_CONF_HD_HIGH    (0x08) // High dynamic seetting

#define VEML6075_CONF_TRIG       (0x04) // Trigger measurement, clears by itself

#define VEML6075_CONF_AF_OFF     (0x00) // Active force mode disabled (default)
#define VEML6075_CONF_AF_ON      (0x02) // Active force mode enabled (?)

#define VEML6075_CONF_SD_OFF     (0x00) // Power up
#define VEML6075_CONF_SD_ON      (0x01) // Power down

#include <Arduino.h>
#include <Wire.h>

class VEML6075 {

  public:
    VEML6075();
    void begin();

    uint16_t getUVA();
    uint16_t getUVB();
    uint16_t getUVDummy();
    uint16_t getUVcomp1();
    uint16_t getUVcomp2();
    uint16_t getDevID();

  private:
    uint16_t read16(uint8_t reg);
    void write16(uint8_t reg, uint16_t data);

};

#endif
