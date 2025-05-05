#ifndef I2C_H
#define I2C_H

#include "delay.h"

void i2c_init();
uint8_t i2c_slave_start(uint8_t *addr);
void i2c_slave_stop();
uint8_t i2c_slave_write(uint8_t data0);
uint8_t i2c_slave_read();

#endif // I2C_H
