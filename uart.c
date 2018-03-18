/* (c) 2018 ukrkyi */

/*
 * Pins used:
 *	B10 ---> TX
 *	B11 ---> RX
 *
 * Peripherals used:
 *	USART3 @ baudrate 600
 */

#include "uart.h"
#include <stm32f1xx.h>
#include <stddef.h>

static void send_next_char();

static bool tx_running = false;
static char *tx_last = NULL;
//static char tx_buffer[256];

void uart_init()
{
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);

	MODIFY_REG(GPIOB->CRH,
		   GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11,
		   (0x3 << GPIO_CRH_MODE10_Pos) | (0x2 << GPIO_CRH_CNF10_Pos) | (0x1 << GPIO_CRH_CNF11_Pos));
	USART3->BRR = 3750 << USART_BRR_DIV_Mantissa_Pos; // 600 for 72MHz clock
	SET_BIT(USART3->CR1,
		USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE);
	NVIC_EnableIRQ(USART3_IRQn);
}

bool uart_send(char * data)
{
	if (tx_running)
		return false;
	if (*data == '\0')
		return true;
	tx_last = data;
	tx_running = true;
	SET_BIT(USART3->CR1, USART_CR1_TXEIE);
	return true;
}

void send_next_char()
{
	char *tx = tx_last++;

	if (*tx_last == '\0') {
		CLEAR_BIT(USART3->CR1, USART_CR1_TXEIE);
		tx_running = false;
		tx_last = NULL;
	}
	USART3->DR = *tx;
}

void USART3_IRQHandler(void)
{
	uint16_t status = READ_REG(USART3->SR);
	uint8_t data = READ_REG(USART3->DR);

	if (READ_BIT(status, USART_SR_TXE) && tx_running)
		send_next_char();
	if (READ_BIT(status, USART_SR_RXNE))
		uart_byte_received_callback(data, status);
}

__attribute__((weak)) void uart_byte_received_callback(char c, uint16_t status)
{
}
