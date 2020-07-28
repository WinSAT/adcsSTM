/*
 * rcc.c
 *
 *      Author: adam
 */


#include "../Inc/rcc.h"

/*
 * RCC_PCLK1_get
 * return the clk speed used for APB1 in this case
 *
 * This should be abstracted to some RCC.c file for other peripherals to use
 *
 * Refer to RCC_CFGR (clock configuration register)
 *        SWS (system clock switch status) - bits 2 and 3
 *        HPRE (AHP Prescaler) - bits 4 to 7
 *        PPRE1 (APB1 prescaler) - bits 10 to 12
 */
uint32_t RCC_PCLK1_get(void){
	uint32_t pclk1, freq, ahb_clk_div, apb1_clk_div;
	uint8_t sws, hpre, ppre1;

	/*
	 * sws:   bits 2 to 3   r-shift 2  then mask with 00000011
	 * ppre1: bits 10 to 12 r-shift 10 then mask with 00000111
	 * hre:   bits 4 to 7   r-shift 4  then mask with 00001111
	 */
	sws   = (RCC->RCC_CFGR >> 2)  & 0x3;
	ppre1 = (RCC->RCC_CFGR >> 10) & 0x7;
	hpre  = (RCC->RCC_CFGR >> 4)  & 0xF;

	if (sws == 0) // HSI
		freq = 1600000;
	else if (sws == 1) // HSE
		// HSE (High Speed External) uses X2 crystal oscillator on evaluation board
		freq = 800000;

	// PLL is option 2, NA for our project
    // option 3 is NA in reference manual
	else return 0; // error

	// AHB clk div
	if (hpre < 8) ahb_clk_div = 1; // no clk divider
	else if (hpre == 8) ahb_clk_div = 2;
	else if (hpre == 9) ahb_clk_div = 4;
	else if (hpre == 10) ahb_clk_div = 8;
	else if (hpre == 11) ahb_clk_div = 16;
	else if (hpre == 12) ahb_clk_div = 32;
	else if (hpre == 13) ahb_clk_div = 64;
	else if (hpre == 14) ahb_clk_div = 128;
	else if (hpre == 15) ahb_clk_div = 512;
	else return 0; // error

	if (ppre1 < 4) apb1_clk_div = 1; // no clk divider
	else if (ppre1 == 4) apb1_clk_div = 2;
	else if (ppre1 == 5) apb1_clk_div = 4;
	else if (ppre1 == 6) apb1_clk_div = 8;
	else if (ppre1 == 7) apb1_clk_div = 16;
	else return 0; // error

	pclk1 = (freq / ahb_clk_div) / apb1_clk_div;
	return pclk1;
}
