#ifndef __IIC_H_
#define __IIC_H_
#include "MPC5604B_0M27V_0102.h" /* Use proper include file */

void init_I2C(void);
void I2C_start();
uint8_t I2C_send(uint8_t data);
uint8_t I2C_receive(uint8_t txak);
void I2C_stop(void);
void I2C_restart(void);

uint8_t writebyte(uint8_t SlaveAddress,uint8_t address, uint8_t thedata);
uint8_t readbyte(uint8_t SlaveAddress,uint8_t address);

#endif
