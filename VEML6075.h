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

#include <Arduino.h>
#include <Wire.h>

#define VEML6075_ADDR  0x10
#define VEML6075_DEVID 0x26

// Reading the application note on calculation of UV index, the "dummy" channel
// value is actually not a dummy value at all, but the dark current count.
// NAMES ARE IMPORTANT PEOPLE.

#define VEML6075_REG_CONF        (0x00) // Configuration register (options below)
#define VEML6075_REG_UVA         (0x07) // UVA register
#define VEML6075_REG_DUMMY       (0x08) // Dark current register (NOT DUMMY)
#define VEML6075_REG_UVB         (0x09) // UVB register
#define VEML6075_REG_UVCOMP1     (0x0A) // Visible compensation register
#define VEML6075_REG_UVCOMP2     (0x0B) // IR compensation register
#define VEML6075_REG_DEVID       (0x0C) // Device ID register

#define VEML6075_CONF_IT_50MS    (0x00) // Integration time = 50ms (default)
#define VEML6075_CONF_IT_100MS   (0x10) // Integration time = 100ms
#define VEML6075_CONF_IT_200MS   (0x20) // Integration time = 200ms
#define VEML6075_CONF_IT_400MS   (0x30) // Integration time = 400ms
#define VEML6075_CONF_IT_800MS   (0x40) // Integration time = 800ms
#define VEML6075_CONF_IT_MASK    (0x8F) // Mask off other config bits

#define VEML6075_CONF_HD_NORM    (0x00) // Normal dynamic seetting (default)
#define VEML6075_CONF_HD_HIGH    (0x08) // High dynamic seetting

#define VEML6075_CONF_TRIG       (0x04) // Trigger measurement, clears by itself

#define VEML6075_CONF_AF_OFF     (0x00) // Active force mode disabled (default)
#define VEML6075_CONF_AF_ON      (0x02) // Active force mode enabled (?)

#define VEML6075_CONF_SD_OFF     (0x00) // Power up
#define VEML6075_CONF_SD_ON      (0x01) // Power down

// To calculate the UV Index, a bunch of empirical/magical coefficients need to
// be applied to UVA and UVB readings to get a proper composite index value.
// Seems pretty hand wavey, though not nearly as annoying as the dark current
// not being subtracted out by default.

#define VEML6075_UVI_UVA_VIS_COEFF (2.22) // aka coeff "A"
#define VEML6075_UVI_UVA_IR_COEFF  (1.33) // aka coeff "B"
#define VEML6075_UVI_UVB_VIS_COEFF (2.95) // aka coeff "C"
#define VEML6075_UVI_UVB_IR_COEFF  (1.74) // aka coeff "D"

// Once the above offsets and crunching is done, there's a last weighting
// function to convert the ADC counts into the UV index values. This handles
// both the conversion into irradiance (W/m^2) and the skin erythema weighting
// by wavelength--UVB is way more dangerous than UVA, due to shorter
// wavelengths and thus more energy per photon. These values convert the
// compensated values.
//
// NB These are the "open air" values given in the application note for the
// VEML6075.

#define VEML6075_UVI_UVA_RESPONSE (0.001461)
#define VEML6075_UVI_UVB_RESPONSE (0.002591)

typedef enum {
  VEML6075_IT_50MS,
  VEML6075_IT_100MS,
  VEML6075_IT_200MS,
  VEML6075_IT_400MS,
  VEML6075_IT_800MS
} veml6075_int_time;

typedef union {
  struct {
    uint8_t SD : 1;
    uint8_t UV_AF : 1;
    uint8_t UV_TRIG : 1;
    uint8_t UV_HD : 1;
    uint8_t UV_IT : 3;
    uint8_t high_byte;
  } bit;
  uint16_t reg;
} veml6075_commandRegister;

class VEML6075 {

  public:

    VEML6075();
    bool begin(veml6075_int_time itime = VEML6075_IT_100MS,bool highDynamic = false, bool forcedReads = false, TwoWire *i2c=&Wire);

    void poll();
    float getUVA();
    float getUVB();
    float getUVIndex();
    uint16_t getDevID();

    uint16_t getRawUVA();
    uint16_t getRawUVB();
    uint16_t getRawDark();
    uint16_t getRawVisComp();
    uint16_t getRawIRComp();
  private:

    TwoWire *i2c;
    
    uint16_t _read_delay;
    veml6075_commandRegister _commandRegister;

    uint16_t raw_uva;
    uint16_t raw_uvb;
    uint16_t raw_dark;
    uint16_t raw_vis;
    uint16_t raw_ir;

    uint16_t read16(uint8_t reg);
    void write16(uint8_t reg, uint16_t data);
};

#endif
