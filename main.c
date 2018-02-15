#include <stm32f1xx.h>

void EXTI9_5_IRQHandler(void)
{
	if (GPIOB->IDR & GPIO_IDR_IDR8)
	{
		GPIOC->BSRR = GPIO_BSRR_BS13;
	}
	else
	{
		GPIOC->BSRR = GPIO_BSRR_BR13;
	}
}

int main(void)
{
	// Port C led setup
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;

	// Switch port setup
	GPIOB->CRH = (GPIOB->CRH & ~GPIO_CRH_CNF8) | (0x2 << GPIO_CRH_CNF8_Pos);
	GPIOB->ODR |= GPIO_ODR_ODR8;
	// Interrupt enable
	EXTI->IMR |= EXTI_IMR_IM8;
	EXTI->RTSR |= EXTI_RTSR_RT8;
	EXTI->FTSR |= EXTI_FTSR_FT8;

	// NVIC Configuration
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	while (1) {
		;
	}
	return 0;
}
