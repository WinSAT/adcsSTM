/*
 * master_send_test.c
 *
 *      Author: adam
 *
 *      This is to test master send data
 *      The STM32 communicates to an Arudino i2c slave
 */

/* Pins:
 * SCL - PB8
 * SDA - PB9
 */

#include <stdio.h>
#include <string.h>
#include "../drivers/Inc/mcu.h"
#include "../drivers/Inc/gpio.h"
#include "../drivers/Inc/i2c.h"
#include "../Inc/master_send.h"

I2C_control_t I2C1_comm;

void I2C1_init_pins(void)
{
	GPIO_control_t i2c_pins;

	i2c_pins.gpio_regs = GPIOB;
	i2c_pins.config.GPIO_Mode = GPIO_MODE_ALTFUNC; // alternating function type
	i2c_pins.config.GPIO_Output = GPIO_OUTPUT_OD; // open drain output type
	i2c_pins.config.GPIO_PUPD = GPIO_PIN_PU; // internal pullup resistor
	i2c_pins.config.GPIO_AltFunc = GPIO_AF4; // alternate function mode is 4
	i2c_pins.config.GPIO_Speed = GPIO_SPEED_FAST;

	// scl
	i2c_pins.config.GPIO_Pin = GPIO_PIN_8;
	GPIO_Init(&i2c_pins);

	// sda
	i2c_pins.config.GPIO_Pin = GPIO_PIN_9;
	GPIO_Init(&i2c_pins);
}

void I2C1_init_config(void)
{
	I2C1_comm.i2c_regs = I2C1;
	I2C1_comm.config.I2C_ACK = I2C_ACK_ENABLE;
	I2C1_comm.config.I2C_DeviceAddress = MASTER_ADDR; // NA since STM32 is master
	I2C1_comm.config.I2C_FM = FMPI2C_DUTY_CYCLE_2; // NA since we're using standard mode
	I2C1_comm.config.I2C_SCL = SCL_DEFAULT;

	I2C_Init(&I2C1_comm);
}

void master_send_init(void)
{
	I2C1_init_pins();
	I2C1_init_config();
	I2C_Enable_Disable(I2C1, TRUE);
}

void master_send_msg(void)
{
	uint8_t msg[] = "STM Master send to Arduino Slave\n";
	I2C_MasterSend(&I2C1_comm, msg, strlen((char*)msg), SLAVE_ADDR);
}
