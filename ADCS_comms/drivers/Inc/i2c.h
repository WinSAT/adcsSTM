/*
 * i2c.h
 *
 *      i2c driver header file
 *
 *      Author: Adam Al-Khazraji
 *
 */

#ifndef DRIVERS_INC_I2C_H_
#define DRIVERS_INC_I2C_H_

#include "mcu.h"

typedef struct {
	uint32_t I2C_SCL; // SCL speed or frequency
	uint8_t I2C_DeviceAddress; // device address on I2C bus
	uint8_t I2C_ACK; // ACK after every message
	uint16_t I2C_FM; // Duty Cycle when FM in (Fast Mode)
}I2C_config_t;

typedef struct {
	I2C_regs_t* i2c_regs; // i2c register structure
	I2C_config_t config; // options for i2c comm
}I2C_control_t;

#define SCL_DEFAULT 100000 // SCL default to 100KHz
#define SCL_FMPI2C  400000 // Fast Mode Plus I2C is between 100KHz to 400KHz

// ACK control bit is bit 10 of I2C CR1 register
#define I2C_ACK_ENABLE  1
#define I2C_ACK_DISABLE 0 // default

// DUTY control bit is bit 14 of I2C CCR register
#define FMPI2C_DUTY_CYCLE_2     0
#define FMPI2C_DUTY_CYCLE_16_9  1

/* I2C_SR1 flags*/
#define I2C_SR1_FLAG_SB      (1 << I2C_SR1_SB)
#define I2C_SR1_FLAG_ADDR    (1 << I2C_SR1_ADDR)
#define I2C_SR1_FLAG_BTF     (1 << I2C_SR1_BTF)
#define I2C_SR1_FLAG_ADD10   (1 << I2C_SR1_ADD10)
#define I2C_SR1_FLAG_STOPF   (1 << I2C_SR1_STOPF)
#define I2C_SR1_FLAG_RXNE    (1 << I2C_SR1_RXNE)
#define I2C_SR1_FLAG_TXE     (1 << I2C_SR1_TXE)
#define I2C_SR1_FLAG_BERR    (1 << I2C_SR1_BERR)
#define I2C_SR1_FLAG_ARLO    (1 << I2C_SR1_ARLO)
#define I2C_SR1_FLAG_AF      (1 << I2C_SR1_AF)
#define I2C_SR1_FLAG_OVR     (1 << I2C_SR1_OVR)
#define I2C_SR1_FLAG_TIMEOUT (1 << I2C_SR1_TIMEOUT)

// I2C peripheral clock setup
void I2C_CLK_Enable(I2C_regs_t *i2c_regs, uint8_t enable);

void I2C_Init(I2C_control_t *i2c_control);
void I2C_Close(I2C_regs_t *i2c_regs);

// IRQ - interrupt requests
void I2C_IRQ_Config(uint8_t IRQ, uint8_t enable);
void I2C_IRQ_Priority(uint8_t IRQ, uint32_t priority);

void I2C_Enable_Disable(I2C_regs_t *i2c_regs, uint8_t enable);
uint8_t I2C_GetStatus(I2C_regs_t *i2c_regs, uint32_t flag);

void I2C_Callback(I2C_control_t *i2c_control, uint8_t app_event);

void I2C_MasterSend(I2C_control_t *i2c_control, uint8_t *tx_buf, uint32_t len, uint8_t slave_addr);

#endif /* DRIVERS_INC_I2C_H_ */
