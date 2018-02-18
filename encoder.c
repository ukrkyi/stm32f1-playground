/* (c) 2018 ukrkyi */

#include "interrupt.h"
#include "encoder.h"
#include <stm32f1xx.h>

#define STEP_COUNT 4U

static int prev_cnt;

void exti_line8_callback(void)
{
	button_state_change_callback();
}

void encoder_init()
{
	// Switch port setup
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
	MODIFY_REG(GPIOB->CRH,
		   GPIO_CRH_CNF8 | GPIO_CRH_MODE8,
		   0x2 << GPIO_CRH_CNF8_Pos);
	SET_BIT(GPIOB->ODR, GPIO_ODR_ODR8);

	// External interrupt enable
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);
	MODIFY_REG(AFIO->EXTICR[2],
		   AFIO_EXTICR3_EXTI8_Msk,
		   AFIO_EXTICR3_EXTI8_PB);
	SET_BIT(EXTI->IMR,  EXTI_IMR_IM8);
	SET_BIT(EXTI->RTSR, EXTI_RTSR_RT8);
	SET_BIT(EXTI->FTSR, EXTI_FTSR_FT8);
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	// Timer setup
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);
	TIM4->ARR = FULL_ROTATION * 4 - 1;
	TIM4->CNT = FULL_ROTATION * 2;
	MODIFY_REG(TIM4->CCMR1, TIM_CCMR1_CC1S | TIM_CCMR1_CC2S | TIM_CCMR1_IC1F | TIM_CCMR1_IC2F,
		   TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0 | (0xF << TIM_CCMR1_IC1F_Pos) | (0xF << TIM_CCMR1_IC2F_Pos));
	MODIFY_REG(TIM4->CCER, TIM_CCER_CC1P | TIM_CCER_CC1NP | TIM_CCER_CC2P | TIM_CCER_CC2NP, 0x0);
	MODIFY_REG(TIM4->SMCR, TIM_SMCR_SMS, 0x3 << TIM_SMCR_SMS_Pos);
	SET_BIT(TIM4->CCER, TIM_CCER_CC1E);
	SET_BIT(TIM4->DIER, TIM_DIER_CC1IE);
	SET_BIT(TIM4->CR1, TIM_CR1_CEN);
	NVIC_EnableIRQ(TIM4_IRQn);
}

bool button_is_pressed()
{
	return READ_BIT(GPIOB->IDR, GPIO_IDR_IDR8) == 0;
}

void TIM4_IRQHandler(void)
{
	if (READ_BIT(TIM4->SR, TIM_SR_CC1IF)) {
		int cnt = TIM4->CNT / STEP_COUNT;
		if (READ_BIT(TIM4->CR1, TIM_CR1_DIR)){
			if ((prev_cnt > cnt) || (cnt - prev_cnt > FULL_ROTATION / 2)){
				prev_cnt = cnt;
				encoder_step_callback(true, cnt);
			}
		} else {
			if ((cnt > prev_cnt) || (prev_cnt - cnt > FULL_ROTATION / 2)){
				prev_cnt = cnt;
				encoder_step_callback(false, cnt);
			}
		}
		CLEAR_BIT(TIM4->SR, TIM_SR_CC1IF);
	}
}

__attribute__((weak)) void button_state_change_callback()
{
}

__attribute__((weak)) void encoder_step_callback(bool backward, int position)
{
}
