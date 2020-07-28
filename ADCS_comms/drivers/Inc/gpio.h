/*
 * gpio.h
 *
 *  Created on: Jul 27, 2020
 *      Author: adam
 */

#ifndef DRIVERS_INC_GPIO_H_
#define DRIVERS_INC_GPIO_H_

#include "mcu.h"

typedef struct {
	uint8_t GPIO_Pin; // pin number
	uint8_t GPIO_Mode; // can be both interrupt or output based
	uint8_t GPIO_Speed;
	uint8_t GPIO_PUPD; // pill up pull down
	uint8_t GPIO_Output;
	uint8_t GPIO_AltFunc;
}GPIO_config_t;

typedef struct {
	GPIO_regs_t* gpio_regs;
	GPIO_config_t config;
}GPIO_control_t;

// GPIO pins
#define GPIO_PIN_0 	0
#define GPIO_PIN_1 	1
#define GPIO_PIN_2 	2
#define GPIO_PIN_3 	3
#define GPIO_PIN_4 	4
#define GPIO_PIN_5 	5
#define GPIO_PIN_6 	6
#define GPIO_PIN_7  7
#define GPIO_PIN_8  8
#define GPIO_PIN_9 	9
#define GPIO_PIN_10	10
#define GPIO_PIN_11	11
#define GPIO_PIN_12	12
#define GPIO_PIN_13 13
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15

// pin modes
// two lsb of GPIO_MODER (port mode register)
#define GPIO_MODE_INPUT 	0
#define GPIO_MODE_OUTPUT	1
#define GPIO_MODE_ALTFUNC 	2
#define GPIO_MODE_ANALOG 	3

// pin interrupt modes
#define GPIO_MODE_IT_FT     4 // falling edge
#define GPIO_MODE_IT_RT     5 // rising
#define GPIO_MODE_IT_RFT    6 // rising falling edge

// output type
// GPIO_OTYPER (port output type register) bits 15:0
// look at GPIO circuit in reference manual for more information
#define GPIO_OUTPUT_PP   0 // push-pull register
#define GPIO_OUTPUT_OD   1 // open drain

// speed
// GPIO_OSPEEDR (port output speed register) bits 1:0
#define GPIO_SPEED_LOW    0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST   2
#define GPOI_SPEED_HIGH   3

// pull-up/pull-down resistor
/*
 * we use the MCU internal PuPd resistor for this project
 * Check reference manual for external resistor calculations
 * that would be needed when more devices are connected to the
 * peripheral bus
 */
// GPIO_PUPDR (port pull-up/pull-down register) bits 1:0
#define GPIO_NO_PUPD 0
#define GPIO_PIN_PU  1
#define GPIO_PIN_PD  2
// 3 is reserved in the reference manual

// alternate function
// use these macros for the high and low registers
// GPIO_AFHR GPIO_AFLR
#define GPIO_AF0  0
#define GPIO_AF1  1
#define GPIO_AF2  2
#define GPIO_AF3  3
#define GPIO_AF4  4
#define GPIO_AF5  5
#define GPIO_AF6  6
#define GPIO_AF7  7
#define GPIO_AF8  8
#define GPIO_AF9  9
#define GPIO_AF10 10
#define GPIO_AF11 11
#define GPIO_AF12 12
#define GPIO_AF13 13
#define GPIO_AF14 14
#define GPIO_AF15 15

/* Only use init as we are using the GPIO ports
 * to configure them as I2C pins
 */
void GPIO_ClkEnable(GPIO_regs_t* gpio_regs, uint8_t enable);
void GPIO_Init(GPIO_control_t* gpio);

#endif /* DRIVERS_INC_GPIO_H_ */
