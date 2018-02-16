/* (c) 2018 ukrkyi */

#include "system.h"
#include <stm32f1xx.h>

void set_system_clock(void)
{
	/* Set HSEON bit */
	SET_BIT(RCC->CR, RCC_CR_HSEON);

	// Wait for HSE to become ready
	while (!READ_BIT(RCC->CR, RCC_CR_HSERDY));

	// Configure PLL
	SET_BIT(RCC->CFGR, RCC_CFGR_PLLMULL9 | RCC_CFGR_PLLSRC | RCC_CFGR_PPRE1_DIV2);

	// Enable PLL
	SET_BIT(RCC->CR, RCC_CR_PLLON);

	// Wait for PLL to become ready
	while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY));

	// Important: Set up latency & prefetch buffer before we speed up SYSCLK
	SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE | 0x2);

	// Set PLL as the clock source
	SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

	// Wait for PLL to become clock source
	while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	SystemCoreClockUpdate();
}
