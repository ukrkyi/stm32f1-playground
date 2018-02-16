/* (c) 2018 ukrkyi */
#include "led.h"
#include <stm32f1xx.h>

void led_init()
{
	// Port C setup
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPCEN);

	MODIFY_REG(GPIOC->CRH,
		   GPIO_CRH_MODE13 | GPIO_CRH_CNF13,
		   (0x2 << GPIO_CRH_MODE13_Pos));
}

void led_off()
{
	GPIOC->BSRR = GPIO_BSRR_BS13;
}

void led_on()
{
	GPIOC->BSRR = GPIO_BSRR_BR13;
}

void led_toggle()
{
	GPIOC->ODR ^= GPIO_ODR_ODR13;
}
