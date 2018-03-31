/* (c) 2018 ukrkyi */

/*
 * Driver for SPI transmission.
 *
 * Pins used:
 *	A5 ---> SCK1
 *	A7 ---> MOSI1
 *
 * Peripherals used:
 *	SPI1 @ 9 MHz
 *
 */
#include "spi.h"
#include <stm32f1xx.h>

void spi_init(int count, int div)
{
	// Calculate divider
	uint16_t divider = 0;

	while (div > 2 && divider < 7) {
		divider++;
		div >>= 1;
	}

	// Clocks enable
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN);

	// SPI configuration
	CLEAR_BIT(SPI1->CR1, SPI_CR1_SPE);
	if (count == 2)
		SET_BIT(SPI1->CR1, SPI_CR1_DFF);
	else
		CLEAR_BIT(SPI1->CR1, SPI_CR1_DFF);
	MODIFY_REG(SPI1->CR1, SPI_CR1_BR,
		   (divider << SPI_CR1_BR_Pos) | SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_SSM | SPI_CR1_SSI); // Fclk / 8
	NVIC_EnableIRQ(SPI1_IRQn);

	// Pins configuration
	MODIFY_REG(GPIOA->CRL, GPIO_CRL_CNF5 | GPIO_CRL_CNF7,
		   (0x2 << GPIO_CRL_CNF5_Pos)  | (0x2 << GPIO_CRL_CNF7_Pos) |
		   (0x3 << GPIO_CRL_MODE5_Pos) | (0x3 << GPIO_CRL_MODE7_Pos));
}

bool spi_send(uint16_t data)
{
	if (!READ_BIT(SPI1->SR, SPI_SR_TXE))
		return false;
	SPI1->DR = data;
	SET_BIT(SPI1->CR2, SPI_CR2_TXEIE);
	return true;
}

bool spi_is_tx()
{
	if (READ_BIT(SPI1->SR, SPI_SR_BSY | SPI_SR_TXE) != SPI_SR_TXE)
		return true;
	else
		return false;
}

void SPI1_IRQHandler()
{
	CLEAR_BIT(SPI1->CR2, SPI_CR2_TXEIE);
	spi_tx_cplt();
}

__attribute__((weak)) void spi_tx_cplt(){}
