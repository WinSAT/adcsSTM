/*
 * rcc.h
 *
 *      Author: adam
 */

#ifndef DRIVERS_INC_RCC_H_
#define DRIVERS_INC_RCC_H_

#include "mcu.h"

/*
 * Peripheral Clock enable for I2C peripheral on APB1 bus
 *  - APB1 is default 16MHz here as we use the HSI (High Speed Internal)
 *    clock, that is the RC (resister capacitor) circuit to generate square wave
 *
 *    This is done in macro functions so we don't pass RCC
 *    memory map through user space
 */
#define GPIOA_CLK_ENABLE() (RCC->RCC_AHB1ENR |= (1 << 0)) // set GPIOAEN bit
#define GPIOB_CLK_ENABLE() (RCC->RCC_AHB1ENR |= (1 << 1)) // set GPIOBEN bit

#define I2C1_CLK_ENABLE() (RCC->RCC_APB1ENR |= (1 << 21)) // set I2C1EN bit
#define I2C2_CLK_ENABLE() (RCC->RCC_APB1ENR |= (1 << 22)) // set I2C2EN bit
#define I2C3_CLK_ENABLE() (RCC->RCC_APB1ENR |= (1 << 23)) // set I2C3EN bit

uint32_t RCC_PCLK1_get(void);

#endif /* DRIVERS_INC_RCC_H_ */
