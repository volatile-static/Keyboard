#ifndef I2C_H
#define I2C_H

#include<stdint.h>
#include<stdbool.h>

#define I2C_WAIT(twi) while (nrfx_twim_is_busy(&twi))

void i2c_writeBuffer(uint8_t addr, uint8_t *buf, uint8_t len);
void i2c_writeByte(uint8_t addr, uint8_t reg, uint8_t dat);
uint8_t i2c_readByte(uint8_t addr, uint8_t reg);
void i2c_init(void);

#endif
