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

bool systick_init(uint32_t ticks, bool div8)
{
	if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
		return false;

	SysTick->LOAD  = (uint32_t)(ticks - 1UL);
	NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL);
	SysTick->VAL   = 0UL;
	SysTick->CTRL  = (div8 ? 0 : SysTick_CTRL_CLKSOURCE_Msk) |
			 SysTick_CTRL_TICKINT_Msk   |
			 SysTick_CTRL_ENABLE_Msk;
	return true;
}
