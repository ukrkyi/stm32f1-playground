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

void USART3_IRQHandler(void)
{
	uint16_t status = USART3->SR;
	uint8_t data = USART3->DR;
	if ((status & USART_SR_RXNE) && (status & USART_SR_TXE))
	{
		USART3->DR = data;
	}
}

void USART3_Send(char * data)
{
	while (*data != '\0') {
		while ((USART3->SR & USART_SR_TXE) == 0x0);
		USART3->DR = *data;
		++data;
	}
}

void SetSystemCoreClock(void)
{
	/* Set HSEON bit */
	RCC->CR |= RCC_CR_HSEON;

	// Wait for HSE to become ready
	while (!(RCC->CR & RCC_CR_HSERDY));

	// Configure PLL
	RCC->CFGR |= RCC_CFGR_PLLMULL9 | RCC_CFGR_PLLSRC | RCC_CFGR_PPRE1_DIV2;

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;

	// Wait for PLL to become ready
	while (!(RCC->CR & RCC_CR_PLLRDY));

	// Important: Set up latency & prefetch buffer before we speed up SYSCLK
	FLASH->ACR = FLASH_ACR_PRFTBE | 0x2;

	// Set PLL as the clock source
	RCC->CFGR |= RCC_CFGR_SW_PLL;

	// Wait for PLL to become clock source
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	SystemCoreClockUpdate();
}

int main(void)
{
	SetSystemCoreClock();

	// Set up LSE
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	PWR->CR |= PWR_CR_DBP;
	/* Set LSEON bit */
	RCC->BDCR |= RCC_BDCR_LSEON;

	// Wait for LSE to become ready
	while (!(RCC->BDCR & RCC_BDCR_LSERDY));

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
	
	// USART setup
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	GPIOB->CRH |= GPIO_CRH_MODE10_1;
	MODIFY_REG(GPIOB->CRH, GPIO_CRH_CNF10_Msk, GPIO_CRH_CNF10_1);
	USART3->BRR = (3750 << USART_BRR_DIV_Mantissa_Pos); // 600 for 36MHz clock
	USART3->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART3_IRQn);

	USART3_Send("Hello!\n");

	while (1) {
		;
	}
	return 0;
}
