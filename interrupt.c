/* (c) 2018 ukrkyi */
#include "interrupt.h"
#include <stm32f1xx.h>

void EXTI9_5_IRQHandler(void)
{
	if (READ_BIT(EXTI->PR, EXTI_PR_PIF8)) {
		EXTI->PR = EXTI_PR_PIF8;
		exti_line8_callback();
	}
}

__attribute__((weak)) void exti_line8_callback()
{
}
