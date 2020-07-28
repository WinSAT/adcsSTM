/*
 * i2c.c
 *
 *   i2c driver source code
 *
 *      Author: Adam Al-Khazraji
 */

#include "../Inc/i2c.h"
#include "../Inc/rcc.h"

/******* local function declarations *******/
static void I2C_Start(I2C_regs_t* i2c_regs);
static void I2C_Stop(I2C_regs_t* i2c_regs);
static void I2C_SendAddr(I2C_regs_t* i2c_regs, uint8_t slave_addr);
static void I2C_ClearADDRFlag(I2C_regs_t* i2c_regs);

/*
 * I2C_Enable_Disable
 * toggle peripheral enable bit I2C_CR1_PE (bit 0) of I2C_CR1 (control reg)
 */
void I2C_Enable_Disable(I2C_regs_t* i2c_regs, uint8_t enable)
{
	if (enable == TRUE)
		i2c_regs->CR1 |= (1 << I2C_CR1_PE);
	else
		i2c_regs->CR1 &= ~(1 << I2C_CR1_PE);
}

/*
 * I2C_CLK_ENABLE
 *
 * set I2CEN bit for the I2C peripheral
 *  - for I2C1, I2C2, and I2C3: RCC_APB1ENR bit at 21, 22, and 23 respectively
 */
void I2C_CLK_ENABLE(I2C_regs_t* i2c_regs, uint8_t enable)
{
	if (enable == TRUE){
		if (i2c_regs == I2C1)
			I2C1_CLK_ENABLE();
		else if (i2c_regs == I2C2)
			I2C2_CLK_ENABLE();
		else if (i2c_regs == I2C3)
			I2C3_CLK_ENABLE();
		else
			return;
	}
	else
		return;
}

uint8_t I2C_GetStatus(I2C_regs_t* i2c_regs, uint32_t flag)
{
	if(i2c_regs->SR1 & flag)
		return TRUE;
	else return FALSE;
}

/*
 * I2C_Init
 *
 * For this project, SCL will be init to default modes and speeds
 * The set up is abstracted to an init function so that future groups
 * could toggle the modes of the I2C peripherals
 *
 * I2C_CR2 FREQ field default to 16MHz (from APB1 using HSI clk)
 * I2C_CCR CCR field calculated to be 80 (for 100KHz default SCL and FREQ 16MHz)
 *
 * We want clk stretching enabled since we have another MCU (raspberry Pi)
 *  acting as master to the STM32 board so we know the I2C hardware will take
 *  care of the timing for data transfer
 *
 * FM Duty Cycle standard mode: TLow is 4.7 microsecs and THigh is 4 microsecs
 */
void I2C_Init(I2C_control_t* i2c_control){

	uint32_t tmp = 0;
	uint16_t ccr = 0; // for 12 bit CCR field in I2C_CCR

	// enable peripheral clk
	I2C_CLK_ENABLE(i2c_control->i2c_regs, TRUE);

	/**** I2C_CR1 ****/
	tmp |= i2c_control->config.I2C_ACK << I2C_CR1_ACK; // bit 10 (ACK control bit)
	i2c_control->i2c_regs->CR1 = tmp;  // set CR1 in register map

	/**** I2C_CR2 ****/
	// get how many MHz then mask with 111111 for first 6 bits
	tmp = (RCC_PCLK1_get()/1000000U) & 0x3F;// set FREQ bits
	i2c_control->i2c_regs->CR2 = tmp; // set CR2 in register map

	/**** I2C_OAR1 ****/
	// Note - we only use 7 bit slave address for this project
	// shift by 1 due to lsb being ADD0 which is NA for 7 bit address
	tmp = i2c_control->config.I2C_DeviceAddress << I2C_OAR1_OFFSET_ADD0;

	// reference manual states bit 14 must always be 1 (reserved)
	tmp |= (1 << I2C_OAR1_RESERVED);

	// bit 15 - ADDMODE must be kept 0 for 7 bit address mode
	// set OAR1 in register map (need this address when in slave mode)
	i2c_control->i2c_regs->OAR1 = tmp;

	/**** I2C_CCR ****/
    // CCR field is bit 0 to 11
	// control bit 15 F/S is 0 for standard mode used for this project
	tmp = 0;
	if(i2c_control->config.I2C_SCL <= SCL_DEFAULT)
	{
		ccr = RCC_PCLK1_get()/(2 * i2c_control->config.I2C_SCL); // multiply by 2 from standard mode ccr formula
		tmp |= (ccr & 0xFFF); // first 12 bits
	}
	else{ // Fast Mode
		tmp |= (1 << I2C_CCR_FS); // set F/S to Fast Mode (bit 15)
		tmp |= (i2c_control->config.I2C_FM << I2C_CCR_DUTY); // set DUTY (bit 14) to given FM duty cycle
		if(i2c_control->config.I2C_FM == FMPI2C_DUTY_CYCLE_2)
			ccr = RCC_PCLK1_get()/(3 * i2c_control->config.I2C_SCL); // DUTY is 2
		else
			ccr = RCC_PCLK1_get()/(25 * i2c_control->config.I2C_SCL); // DUTY is 16/9

		// set CCR field and mask to first 12 bits
		tmp |= (ccr & 0xFFF);
	}
	// set CCR is register map
	i2c_control->i2c_regs->CCR = tmp;

	/**** I2C_TRISE ****/
	if(i2c_control->config.I2C_SCL <= SCL_DEFAULT)
	{
		tmp = (RCC_PCLK1_get() / 1000000U) + 1; // add 1 from reference manual
	}
	else{ // fast mode
		tmp = ((RCC_PCLK1_get() * 300) / 1000000000U) + 1;
	}

	i2c_control->i2c_regs->TRISE = (tmp & 0x3F); // 6 bit mask

	return;
}

void I2C_MasterSend(I2C_control_t* i2c_control, uint8_t* tx_buf, uint32_t len, uint8_t slave_addr)
{
	// start condition
	I2C_Start(i2c_control->i2c_regs);

	// wait for SB (start bit)
	while(!(I2C_GetStatus(i2c_control->i2c_regs, I2C_SR1_FLAG_SB)));

	I2C_SendAddr(i2c_control->i2c_regs, slave_addr);

	I2C_ClearADDRFlag(i2c_control->i2c_regs);

	// send data until len is 0
	for (; len > 0; len--){
		while(!(I2C_GetStatus(i2c_control->i2c_regs, I2C_SR1_FLAG_TXE))); // wait for TxE
		i2c_control->i2c_regs->DR = *tx_buf; // dereference for value
		tx_buf++; // increment position
	}

	// wait for TxE and BTF in I2C_SR1 then set STOP to 1 in I2C_CR1
	while(!(I2C_GetStatus(i2c_control->i2c_regs, I2C_SR1_FLAG_TXE)));
	while(!(I2C_GetStatus(i2c_control->i2c_regs, I2C_SR1_FLAG_BTF)));
	I2C_Stop(i2c_control->i2c_regs);

	return;
}

// i2c start condition
static void I2C_Start(I2C_regs_t* i2c_regs){
	/* START bit 8 in I2C_CR1
	 * set to 1 for repeated start generation
	 * if PE is 0, the i2c hardware will clear START bit
	 */
	i2c_regs->CR1 |= (1 << I2C_CR1_START);

	return;
}

// i2c stop condition
static void I2C_Stop(I2C_regs_t* i2c_regs){
	// I2C_CR1 STOP (bit 9)
	// when bit set to 1:
	//    "Stop generation after the current byte transfer or after the current Start condition is sent"
	i2c_regs->CR1 |= (1 << I2C_CR1_STOP);
}

// send address with r/w bit set to 0
static void I2C_SendAddr(I2C_regs_t* i2c_regs, uint8_t slave_addr)
{
	slave_addr = (slave_addr << 1); // shift left for r/w_ bit
	slave_addr &= ~(1); // set r/w_ bit to 0

	/* I2C_DR (data register) has DR field from bits 0 to 7
	 * "Transmitter mode: Byte transmission starts automatically when a byte is written in the DR register" - 24.6.5
	 */
	i2c_regs->DR = slave_addr;

	return;
}

/* ADDR bit is SR1
 * "This bit is cleared by software reading SR1 register followed reading SR2
 * or by hardware when PE=0" - 24.6.6
 */
static void I2C_ClearADDRFlag(I2C_regs_t* i2c_regs){
	uint32_t foo = i2c_regs->SR1;
	foo = i2c_regs->SR2;
	(void)foo;
}

