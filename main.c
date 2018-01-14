#include <stm32f1xx.h>

int main(void)
{
	// Port C led setup
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
	while (1) {
		GPIOC->ODR ^= GPIO_ODR_ODR13;
		for (volatile int i = 0; i < 1000000; i++);
	}
	return 0;
}
