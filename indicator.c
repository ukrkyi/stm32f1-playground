/* (c) 2018 ukrkyi */
#include "indicator.h"
#include "shift.h"
#include "encoder.h"
#include "letter.h"

#include <stm32f1xx.h>

// Variables

static uint8_t matrix[8];
static uint8_t position = 0;

// Function declarations

static inline int max(int a, int b);
static inline int min(int a, int b);

// Function definitions

void indicator_init()
{
	shift_init(2);
	shift_set(0xFFFF);
}

void indicator_setpoint(int x, int y)
{
	matrix[y] |= 1 << x;
}

void indicator_clearpoint(int x, int y)
{
	matrix[y] &= ~(1 << x);
}

void indicator_clear()
{
	for (int i = 0; i < 8; i++)
		matrix[i] = 0;
}

int indicator_putletter(unsigned char letter, int offset)
{
	int i;

	for (i = max(offset, 0); i < min(offset + alpha[letter].w, 8); i++)
		matrix[i] = alpha[letter].letter[i - offset];

	return alpha[letter].w;
}

void indicator_tick()
{
	shift_set((0xFF00 & ~(1 << position << 8)) | (matrix[position]));
	position = (position + 1) % 8;
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
