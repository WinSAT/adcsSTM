/*
 * mcu.h
 *
 * MCU specific header file for STM32-F446RE
 *
 * Refer to reference manual to see memory, register maps, and control bits:
 *     RM0390 Reference Manual - STM32F446xx advanced ARM-based 32-bit MCUs
 *
 *      Author: Adam Al-Khazraji
 */

#ifndef DRIVERS_INC_MCU_H_
#define DRIVERS_INC_MCU_H_

#include <stdint.h>

/************* General Macros ***************/
#define TRUE  1
#define FALSE 0

/************* AHB/APB Bridges **************/

/* base address of APB1 (Advanced Peripheral Bus)
* that contains the CPU's I2C registers in it's memory map
*/
#define APB1 0x40000000U

/* base address of AHB1 (Advanced High-performance Bus)
 * that contains the CPU's RCC registers in it's memory map
 */
#define AHB1 0x40020000U
/*******************************************/

/************** Memory Maps ****************/

/* Base address of RCC (Reset and Clock Control)
* control registers in the CPU memory map
*/
#define RCC_ADDR (AHB1 + 0x3800U)

/* Base address of GPIO ports on the AHB1 bus
 * that can be configures as I2C pins
 */
#define GPIOA_ADDR (AHB1 + 0x0000)
#define GPIOB_ADDR (AHB1 + 0x0400)

/* Base addresses of I2C (Inter-Integrated Circuit)
 * control registers in the CPU memory
 */
#define I2C1_ADDR (APB1 + 0x5400U)
#define I2C2_ADDR (APB1 + 0x5800U)
#define I2C3_ADDR (APB1 + 0x5C00U)
/*********************************************/

/************** Register Maps ****************/

// RCC register map
typedef struct {
	volatile uint32_t RCC_CR;
	volatile uint32_t RCC_PLLCFGR;
	volatile uint32_t RCC_CFGR;
	volatile uint32_t RCC_CIR;
	volatile uint32_t RCC_AHB1RSTR;
	volatile uint32_t RCC_AHB2RSTR;
	volatile uint32_t RCC_AHB3RSTR;
	uint32_t Reserved_1C;
	volatile uint32_t RCC_APB1RSTR;
	volatile uint32_t RCC_APB2RSTR;
	uint32_t Reserved_28;
	uint32_t Reserved_2C;
	volatile uint32_t RCC_AHB1ENR;
	volatile uint32_t RCC_AHB2ENR;
	volatile uint32_t RCC_AHB3ENR;
	volatile uint32_t RCC_APB1ENR; // use
	volatile uint32_t RCC_APB2ENR;
	uint32_t Reserved_48;
	uint32_t Reserved_4C;
	volatile uint32_t RCC_AHB1LPENR;
	volatile uint32_t RCC_AHB2LPENR;
	volatile uint32_t RCC_AHB3LPENR;
	uint32_t Reserved_5C;
	volatile uint32_t RCC_APB1LPER;
	volatile uint32_t RCC_APB2LPENR;
	uint32_t Reserved_68;
	uint32_t Reserved_6C;
	volatile uint32_t RCC_BDCR;
	volatile uint32_t RCC_CSR;
	uint32_t Reserved_78;
	uint32_t Reserved_7C;
	volatile uint32_t RCC_SSCGR;
	volatile uint32_t RCC_PLLI2SCFGR;
	volatile uint32_t RCC_PLLSAICFGR;
	volatile uint32_t RCC_DCKCFGR;
	volatile uint32_t RCC_CKGATENR;
	volatile uint32_t RCC_DCKCFGR2;
}RCC_regs_t;

// GPIO register map
typedef struct {
	volatile uint32_t GPIO_MODER; // port mode
	volatile uint32_t GPIO_OTYPER; // port output type
	volatile uint32_t GPIO_OSPEEDR; // port output speed
	volatile uint32_t GPIO_PUPDR; // port pull-up/pull-down
	volatile uint32_t GPIO_IDR; // port input data
	volatile uint32_t GPIO_ODR; // port output data
	volatile uint32_t GPIO_BSRR; // port bit set/reset
	volatile uint32_t GPIO_LCKR; // port configuration lock
	volatile uint32_t GPIO_AFRL; // alternate function low
	volatile uint32_t GPIO_AFRH; // alternate function high
}GPIO_regs_t;

// I2C register map
typedef struct {
	volatile uint32_t CR1; // use
	volatile uint32_t CR2; // use
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1; // use
	volatile uint32_t SR2; // use
	volatile uint32_t CCR;  // use
	volatile uint32_t TRISE;
	volatile uint32_t FLTR;
}I2C_regs_t;

/* Register map pointers to register
 * map structures in memory
 */
#define RCC   ((RCC_regs_t*)RCC_ADDR)
#define GPIOA ((GPIO_regs_t*)GPIOA_ADDR)
#define GPIOB ((GPIO_regs_t*)GPIOB_ADDR)
#define I2C1  ((I2C_regs_t*)I2C1_ADDR)
#define I2C2  ((I2C_regs_t*)I2C2_ADDR)
#define I2C3  ((I2C_regs_t*)I2C3_ADDR)
/*********************************************/

/******** I2C registers bit positions ********/

// I2C_CR1 (control register 1) bit positions
#define I2C_CR1_PE        0
#define I2C_CR1_NOSTRETCH 7
#define I2C_CR1_START     8
#define I2C_CR1_STOP      9
#define I2C_CR1_ACK       10
#define I2C_CR1_SWRST     15

// I2C_CR2 bit position
#define I2C_CR2_FREQ    0
#define I2C_CR2_ITERREN 8
#define I2C_CR2_ITEVTEN 9
#define I2C_CR2_ITBUFEN 10

// I2C_OAR1 bit position
#define I2C_OAR1_RESERVED    14
#define I2C_OAR1_OFFSET_ADD0 1

// I2C_SR1 (status register) bit position
#define I2C_SR1_SB      0
#define I2C_SR1_ADDR    1
#define I2C_SR1_BTF     2
#define I2C_SR1_ADD10   3
#define I2C_SR1_STOPF   4
#define I2C_SR1_RXNE    6
#define I2C_SR1_TXE     7
#define I2C_SR1_BERR    8
#define I2C_SR1_ARLO    9
#define I2C_SR1_AF      10
#define I2C_SR1_OVR     11
#define I2C_SR1_TIMEOUT 14

// I2C_SR2 bit positions
#define I2C_SR2_MSL     0
#define I2C_SR2_BUSY    1
#define I2C_SR2_TRA     2
#define I2C_SR2_GENCALL 4
#define I2C_SR2_DUALF   7

// I2C_CCR (clock control register) bit positions
#define I2C_CCR_CCR  0
#define I2C_CCR_DUTY 14
#define I2C_CCR_FS   15
/*********************************************/

#endif /* DRIVERS_INC_MCU_H_ */
