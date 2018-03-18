/* (c) 2018 ukrkyi */

/*
 * Pins used:
 *	C13 ---> LED (on-board, active low)
 *	A8 ----> R |
 *	A9 ----> G |> RGB LED (common cathode connected to ground)
 *	A10 ---> B |
 *
 * Peripherals used:
 *	TIM1 @ 2 MHz / 100
 *	- ARR = 99
 *	- CH 1-3, PWM mode @ 20 kHz
 */

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

void led_rgb_init()
{
	// Port A setup
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);

	WRITE_REG(TIM1->PSC, 0x23);
	SET_BIT(TIM1->CR1, TIM_CR1_ARPE);
	MODIFY_REG(TIM1->CCMR1, TIM_CCMR1_OC1M | TIM_CCMR1_OC2M | TIM_CCMR1_CC1S | TIM_CCMR1_CC2S,
		   (0x6 << TIM_CCMR1_OC1M_Pos) | (0x6 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE);
	MODIFY_REG(TIM1->CCMR2, TIM_CCMR2_OC3M | TIM_CCMR2_CC3S,
		   (0x6 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE);
	WRITE_REG(TIM1->ARR, 99);
	WRITE_REG(TIM1->CCR1, 0);
	WRITE_REG(TIM1->CCR2, 0);
	WRITE_REG(TIM1->CCR3, 0);
	WRITE_REG(TIM1->EGR, TIM_EGR_UG);
	SET_BIT(TIM1->CCER, TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E);
	SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);
	MODIFY_REG(GPIOA->CRH,
		   GPIO_CRH_MODE8 | GPIO_CRH_CNF8 | GPIO_CRH_MODE9 | GPIO_CRH_CNF9 | GPIO_CRH_MODE10 | GPIO_CRH_CNF10,
		   (0x2 << GPIO_CRH_MODE8_Pos) | (0x2 << GPIO_CRH_MODE9_Pos) | (0x2 << GPIO_CRH_MODE10_Pos) |
		   (0x2 << GPIO_CRH_CNF8_Pos) | (0x2 << GPIO_CRH_CNF9_Pos) | (0x2 << GPIO_CRH_CNF10_Pos));
	SET_BIT(TIM1->CR1, TIM_CR1_CEN);
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

void led_rgb_setcolor(unsigned r, unsigned g, unsigned b)
{
	WRITE_REG(TIM1->CCR1, r);
	WRITE_REG(TIM1->CCR2, g);
	WRITE_REG(TIM1->CCR3, b);
}
