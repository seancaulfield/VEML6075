/*
 * VEML6075.cpp
 *
 * Arduino library for the Vishay VEML6075 UVA/UVB i2c sensor.
 *
 * Author: Sean Caulfield <sean@yak.net>
 * 
 * License: GPLv2.0
 *
 */

#include "VEML6075.h"

VEML6075::VEML6075(I2C_HandleTypeDef &hi2c) : i2c(hi2c){

  // Despite the datasheet saying this isn't the default on startup, it appears
  // like it is. So tell the thing to actually start gathering data.


  config = 0;
  config |= VEML6075_CONF_SD_OFF;

  // App note only provided math for this one...
  config |= VEML6075_CONF_IT_100MS;
}

bool VEML6075::begin() {


  if (getDevID() != VEML6075_DEVID) {
    return false;
  }

  // Write config to make sure device is enabled
  write16(VEML6075_REG_CONF, config);

  return true;
}

// Poll sensor for latest values and cache them
void VEML6075::poll() {
  raw_uva = read16(VEML6075_REG_UVA);
  raw_uvb = read16(VEML6075_REG_UVB);
  raw_dark = read16(VEML6075_REG_DUMMY);
  raw_vis = read16(VEML6075_REG_UVCOMP1);
  raw_ir = read16(VEML6075_REG_UVCOMP2);
}

uint16_t VEML6075::getRawUVA() {
  return raw_uva;
}

uint16_t VEML6075::getRawUVB() {
  return raw_uvb;
}

uint16_t VEML6075::getRawDark() {
  return raw_dark;
}

uint16_t VEML6075::getRawVisComp() {
  return raw_vis;
}

uint16_t VEML6075::getRawIRComp() {
  return raw_ir;
}


uint16_t VEML6075::getDevID() {
  return read16(VEML6075_REG_DEVID);
}

float VEML6075::getUVA() {
  //float comp_vis = raw_vis - raw_dark;
  //float comp_ir = raw_ir - raw_dark;
  //float comp_uva = raw_uva - raw_dark;
  float comp_vis;
  float comp_ir;
  float comp_uva;

  // Constrain compensation channels to positive values
  comp_ir  = max(raw_ir - raw_dark, 0.0);
  comp_vis = max(raw_vis - raw_dark, 0.0);
  comp_uva = max(raw_uva - raw_dark, 0.0);

  // Scale by coeffs from datasheet
  comp_vis *= VEML6075_UVI_UVA_VIS_COEFF;
  comp_ir  *= VEML6075_UVI_UVA_IR_COEFF;

  // Subtract out visible and IR components
  comp_uva = max(comp_uva - comp_ir, 0.0);
  comp_uva = max(comp_uva - comp_vis, 0.0);

  return comp_uva;
}

float VEML6075::getUVB() {
  //float comp_vis = raw_vis - raw_dark;
  //float comp_ir = raw_ir - raw_dark;
  //float comp_uvb = raw_uvb - raw_dark;
  float comp_vis;
  float comp_ir;
  float comp_uvb;

  // Constrain compensation channels to positive values
  comp_ir  = max(raw_ir - raw_dark, 0.0);
  comp_vis = max(raw_vis - raw_dark, 0.0);
  comp_uvb = max(raw_uvb - raw_dark, 0.0);

  // Scale by coeffs from datasheet
  comp_vis *= VEML6075_UVI_UVB_VIS_COEFF;
  comp_ir  *= VEML6075_UVI_UVB_IR_COEFF;

  // Subtract out visible and IR components
  comp_uvb = max(comp_uvb - comp_ir, 0.0);
  comp_uvb = max(comp_uvb - comp_vis, 0.0);

  return comp_uvb;
}

float VEML6075::getUVIndex() {
  float uva_weighted = getUVA() * VEML6075_UVI_UVA_RESPONSE;
  float uvb_weighted = getUVB() * VEML6075_UVI_UVB_RESPONSE;
  return (uva_weighted + uvb_weighted) / 2.0;
}

uint16_t VEML6075::read16(uint8_t reg) {
	uint8_t data[2];

	HAL_I2C_Mem_Read(&i2c, VEML6075_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data , 2, 100);

	return (data[1] << 8) | data[0];
}

void VEML6075::write16(uint8_t reg, uint16_t data) {

	uint8_t lsb = (uint8_t)(0xFF & (data >> 0));
	uint8_t msb = (uint8_t)(0xFF & (data >> 8));

	uint8_t buffer[2] = {lsb,msb};

	HAL_I2C_Mem_Write(&i2c, VEML6075_ADDR, reg, I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
}

uint16_t VEML6075::max(uint16_t a, uint16_t b){
	if(a <= b){
		return b;
	}else{
		return a;
	}
}
