/*
 * gpio.c
 *
 *  Created on: Jul 27, 2020
 *      Author: adam
 */

#include "../Inc/gpio.h"
#include "../Inc/rcc.h"

/* Only use init as we are using the GPIO ports
 * to configure them as I2C pins
 */
void GPIO_ClkEnable(GPIO_regs_t* gpio_regs, uint8_t enable)
{
	if(enable == TRUE)
	{
		if(gpio_regs == GPIOA)
			GPIOA_CLK_ENABLE();
		else if (gpio_regs == GPIOB)
			GPIOB_CLK_ENABLE();
	}
	else return;
}

void GPIO_Init(GPIO_control_t* gpio)
{
	uint32_t tmp = 0;
	//enable the peripheral clock

	GPIO_ClkEnable(gpio->gpio_regs, TRUE);

	// pin mode
	// non IT modes
	if(gpio->config.GPIO_Mode <= GPIO_MODE_ANALOG)
	{
		// multiply by 2 since each pin mode is a two bit field in GPIO_MODER
		tmp = (gpio->config.GPIO_Mode << (2 * gpio->config.GPIO_Pin ) );
		gpio->gpio_regs->GPIO_MODER &= ~( 0x3 << (2 * gpio->config.GPIO_Pin)); //clearing
		gpio->gpio_regs->GPIO_MODER |= tmp; //setting
	}
	// else is IT iterrupt modes which is unused for this
	// project as we are focusing on I2C driver

	// speed
	tmp = (gpio->config.GPIO_Speed << (2 * gpio->config.GPIO_Pin));
	gpio->gpio_regs->GPIO_OSPEEDR &= ~( 0x3 << ( 2 * gpio->config.GPIO_Pin)); //clearing
	gpio->gpio_regs->GPIO_OSPEEDR |= tmp;

	//pull-up/pull-down resistor
	tmp = (gpio->config.GPIO_PUPD << ( 2 * gpio->config.GPIO_Pin) );
	gpio->gpio_regs->GPIO_PUPDR &= ~( 0x3 << ( 2 * gpio->config.GPIO_Pin)); //clearing
	gpio->gpio_regs->GPIO_PUPDR |= tmp;

	//output type
	tmp = (gpio->config.GPIO_Output << gpio->config.GPIO_Pin);
	gpio->gpio_regs->GPIO_OTYPER &= ~( 0x1 << gpio->config.GPIO_Pin); //clearing
	gpio->gpio_regs->GPIO_OTYPER |= tmp;

	//5. configure the alt functionality
	if(gpio->config.GPIO_Mode == GPIO_MODE_ALTFUNC)
	{
		uint8_t high_reg =  gpio->config.GPIO_Pin / 8;
		tmp =  gpio->config.GPIO_Pin % 8;

		if(high_reg)
		{
			gpio->gpio_regs->GPIO_AFRH &= ~(0xF << (4 * tmp)); //clear
			gpio->gpio_regs->GPIO_AFRH |= (gpio->config.GPIO_AltFunc << (4 * tmp));
		}
		else { // alternate function reg low
			gpio->gpio_regs->GPIO_AFRL &= ~(0xF << (4 * tmp)); //clear
			gpio->gpio_regs->GPIO_AFRL|= (gpio->config.GPIO_AltFunc << (4 * tmp));
		}
	}

}
