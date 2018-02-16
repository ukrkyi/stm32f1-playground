/* (c) 2018 ukrkyi */

#include "interrupt.h"
#include "encoder.h"
#include <stm32f1xx.h>

void exti_line8_callback(void)
{
	button_state_change_callback();
}

void encoder_init()
{
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);

	// Switch port setup
	MODIFY_REG(GPIOB->CRH,
		   GPIO_CRH_CNF8 | GPIO_CRH_MODE8,
		   0x2 << GPIO_CRH_CNF8_Pos);
	SET_BIT(GPIOB->ODR, GPIO_ODR_ODR8);
	// Interrupt enable
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
	MODIFY_REG(AFIO->EXTICR[2],
		   AFIO_EXTICR3_EXTI8_Msk,
		   AFIO_EXTICR3_EXTI8_PB);
	SET_BIT(EXTI->IMR,  EXTI_IMR_IM8);
	SET_BIT(EXTI->RTSR, EXTI_RTSR_RT8);
	SET_BIT(EXTI->FTSR, EXTI_FTSR_FT8);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
}

bool button_is_pressed()
{
	return READ_BIT(GPIOB->IDR, GPIO_IDR_IDR8) == 0;
}

__attribute__((weak)) void button_state_change_callback()
{
}
