/* (c) 2018 ukrkyi */

/*
 * Pins used:
 *	A5 ---> SCK1 ---> SHCP
 *	A6 ---> GPIO ---> STCP
 *	A7 ---> MOSI1 --> DS
 *
 * Peripherals used:
 *	SPI1 @ 9 MHz
 *
 * Perspective: switch A6 from GPIO to TIM3C1.
 *
 */

#include "shift.h"
#include "spi.h"
#include <stm32f1xx.h>

void shift_init(int count)
{
	// Clocks enable
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
//	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);

//	// Timer configuration
//	WRITE_REG(TIM3->PSC, 0x0);
//	SET_BIT(TIM3->CR1, TIM_CR1_ARPE | TIM_CR1_OPM);
//	MODIFY_REG(TIM3->CCMR1, TIM_CCMR1_OC1M | TIM_CCMR1_CC1S,
//		   (0x7 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE);
//	WRITE_REG(TIM3->ARR, 0x7);
//	WRITE_REG(TIM3->CCR1, 0x4);
//	WRITE_REG(TIM3->EGR, TIM_EGR_UG);
//	SET_BIT(TIM3->CCER, TIM_CCER_CC1E);

	// SPI configuration
	spi_init(2, 8);

	// Pins configuration
	MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF6, (/*0x2*/ 0x0 << GPIO_CRL_CNF6_Pos) | (0x3 << GPIO_CRL_MODE6_Pos));
}

void shift_set(uint16_t data)
{
	while (!spi_send(data));
	while (spi_is_tx());
	GPIOA->BSRR = GPIO_BSRR_BS6;
	GPIOA->BSRR = GPIO_BSRR_BR6;
	//SET_BIT(TIM3->CR1, TIM_CR1_CEN);
}
