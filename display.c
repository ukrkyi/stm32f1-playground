/* (c) 2018 ukrkyi */

/*
 * Pins used:
 *	A3 ---> GPIO ---> RST
 *	A4 ---> GPIO ---> CE
 *	A5 ---> SCK1 ---> CLK
 *	A6 ---> GPIO ---> DC
 *	A7 ---> MOSI1 --> DIN
 *
 * Peripherals used:
 *	SPI1 @ 4.5 MHz
 *
 * Perspective: switch A6 from GPIO to TIM3C1.
 *
 */

// Incluudes

#include "display.h"
#include "spi.h"
#include "letter.h"

#include <stm32f1xx.h>

// Function declaration

static inline void set_cmd_mode();
static inline void set_data_mode();

static inline void cs_enable();
static inline void cs_disable();

static inline int max(int a, int b);
static inline int min(int a, int b);

// Defines

#define LCDWIDTH 84
#define LCDHEIGHT 48

#define LINENUMBER 6

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

// Functions

static int x = 0;

void display_init()
{
	// Port A setup
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);

	MODIFY_REG(GPIOA->ODR, GPIO_ODR_ODR3, GPIO_ODR_ODR4);
	MODIFY_REG(GPIOA->CRL,
		   GPIO_CRL_CNF3 | GPIO_CRL_CNF4 | GPIO_CRL_CNF6,
		   (0x3 << GPIO_CRL_MODE3_Pos) | (0x3 << GPIO_CRL_MODE4_Pos) | (0x3 << GPIO_CRL_MODE6_Pos));

	spi_init(1, 32);

	GPIOA->BSRR = GPIO_BSRR_BS3;

	while (spi_is_tx());
	cs_enable();
	set_cmd_mode();

	// get into the EXTENDED mode!
	while (!spi_send(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION));

	// LCD bias select (4 is optimal?)
	while (!spi_send(PCD8544_SETBIAS | 4));

	// set VOP
	while (!spi_send(PCD8544_SETVOP | 0x3F));

	// normal mode
	while (!spi_send(PCD8544_FUNCTIONSET));

	// Set display to Normal
	while (!spi_send(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL));

	display_clear();
}

void display_clear()
{
	display_set_write_position(0, 0);

	while (spi_is_tx());
	cs_enable();
	set_data_mode();

	for (int i = 0; i < 504; i++)
		while (!spi_send(0x00));

	while (spi_is_tx());
	cs_disable();
}

void display_set_write_position(unsigned line, unsigned offset)
{
	if (line >= LINENUMBER || offset >= LCDWIDTH)
		return;

	while (spi_is_tx());
	cs_enable();
	set_cmd_mode();

	while (!spi_send(PCD8544_SETYADDR | line));
	while (!spi_send(PCD8544_SETXADDR | max(0, offset)));

	while (spi_is_tx());
	cs_disable();
}

int display_putletter(unsigned char letter)
{
	int i;

	while (spi_is_tx());
	cs_enable();
	set_data_mode();

	for (i = 0; i < alpha[letter].w; i++)
		while (!spi_send(alpha[letter].letter[i]));

	while (spi_is_tx());
	cs_disable();

	return alpha[letter].w;
}

void set_cmd_mode()
{
	GPIOA->BSRR = GPIO_BSRR_BR6;
}
void set_data_mode()
{
	GPIOA->BSRR = GPIO_BSRR_BS6;
}

void cs_enable()
{
	GPIOA->BSRR = GPIO_BSRR_BR4;
}
void cs_disable()
{
	GPIOA->BSRR = GPIO_BSRR_BS4;
}

static inline int max(int a, int b)
{
	if (a > b)
		return a;
	else
		return b;
}

static inline int min(int a, int b)
{
	if (a < b)
		return a;
	else
		return b;
}
