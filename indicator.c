/* (c) 2018 ukrkyi */
#include "indicator.h"
#include "shift.h"
#include "encoder.h"

#include <stm32f1xx.h>

// Variables

struct letter {
	uint8_t w;
	uint8_t letter[7];
} const alpha[256] = {
	[0] =			{ .w = 1, .letter = { 0,	 0,	 0,	     0		} },
	[(unsigned char)'\n'] = { .w = 8, .letter = { 0,	 0,	 0,	     0		} },
	[(unsigned char)' '] =	{ .w = 2, .letter = { 0,	 0,	 0,	     0		} },
	[(unsigned char)'-'] =	{ .w = 3, .letter = { 0,	 0,	 0,	     6		} },
	[(unsigned char)'?'] =	{ .w = 3, .letter = { 3,	 4,	 2,	     0, 2	} },
	[(unsigned char)'!'] =	{ .w = 2, .letter = { 2,	 2,	 2,	     0, 2	} },
	[(unsigned char)'.'] =	{ .w = 1, .letter = { 0,	 0,	 0,	     0, 1, 0	} },
	[(unsigned char)':'] =	{ .w = 1, .letter = { 0,	 0,	 1,	     0, 1, 0	} },
	[(unsigned char)','] =	{ .w = 2, .letter = { 0,	 0,	 0,	     0, 2, 1	} },
	[(unsigned char)'"'] =	{ .w = 2, .letter = { 3,	 3,	 0,	     0, 0	} },
	[(unsigned char)'\'']=	{ .w = 1, .letter = { 1,	 1,	 0,	     0, 0	} },
	[(unsigned char)'á'] =	{ .w = 4, .letter = { 0x6 << 1,	 0x5 << 1, 0x7 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'â'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x1 << 1, 0x3 << 1,   0x5 << 1, 0x3 << 1		} },
	[(unsigned char)'÷'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x5 << 1, 0x3 << 1,   0x5 << 1, 0x7 << 1		} },
	[(unsigned char)'ç'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x1 << 1, 0x1 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'½'] =	{ .w = 4, .letter = { 0x4 << 1,	 0x7 << 1, 0x1 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'ä'] =	{ .w = 6, .letter = { 0xC << 1,	 0xA << 1, 0xA << 1,   0xA << 1, 0x1F << 1, 0x11 << 1} },
	[(unsigned char)'å'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x1 << 1, 0x7 << 1,   0x1 << 1, 0x7 << 1		} },
	[(unsigned char)'´'] =	{ .w = 4, .letter = { 0x6 << 1,	 0x1 << 1, 0x7 << 1,   0x1 << 1, 0x6 << 1		} },
	[(unsigned char)'ö'] =	{ .w = 6, .letter = { 0x15 << 1, 0x15<< 1, 0xE << 1,  0x15<< 1, 0x15 << 1} },
	[(unsigned char)'ú'] =	{ .w = 4, .letter = { 0x3 << 1,	 0x4 << 1, 0x3 << 1,   0x4 << 1, 0x3 << 1		} },
	[(unsigned char)'é'] =	{ .w = 5, .letter = { 0x9 << 1,	 0xD << 1, 0xF << 1,   0xB << 1, 0x9 << 1		} },
	[(unsigned char)'¶'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x2 << 1, 0x2 << 1,   0x2 << 1, 0x7 << 1		} },
	[(unsigned char)'·'] =	{ .w = 4, .letter = { 0x5 << 1,	 0x7 << 1, 0x2 << 1,   0x2 << 1, 0x7 << 1		} },
	[(unsigned char)'ê'] =	{ .w = 5, .letter = { 0x6 << 1,	 0x9 << 1, 0xD << 1,   0xB << 1, 0x9 << 1		} },
	[(unsigned char)'ë'] =	{ .w = 4, .letter = { 0x5 << 1,	 0x3 << 1, 0x3 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'ì'] =	{ .w = 4, .letter = { 0x6 << 1,	 0x5 << 1, 0x5 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'í'] =	{ .w = 6, .letter = { 0x11 << 1, 0x1B << 1, 0x15 << 1,  0x15 << 1, 0x11 << 1	} },
	[(unsigned char)'î'] =	{ .w = 4, .letter = { 0x5 << 1,	 0x5 << 1, 0x7 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'ï'] =	{ .w = 4, .letter = { 0x2 << 1,	 0x5 << 1, 0x5 << 1,   0x5 << 1, 0x2 << 1		} },
	[(unsigned char)'ð'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x5 << 1, 0x5 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'ò'] =	{ .w = 4, .letter = { 0x3 << 1,	 0x5 << 1, 0x3 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'ó'] =	{ .w = 4, .letter = { 0x6 << 1,	 0x1 << 1, 0x1 << 1,   0x1 << 1, 0x6 << 1		} },
	[(unsigned char)'ô'] =	{ .w = 4, .letter = { 0x7 << 1,	 0x2 << 1, 0x2 << 1,   0x2 << 1, 0x2 << 1		} },
	[(unsigned char)'õ'] =	{ .w = 4, .letter = { 0x5 << 1,	 0x5 << 1, 0x7 << 1,   0x4 << 1, 0x7 << 1		} },
	[(unsigned char)'æ'] =	{ .w = 6, .letter = { 0xE << 1,  0x15 << 1,0xE << 1,   0x4 << 1, 0x4 << 1} },
	[(unsigned char)'è'] =	{ .w = 4, .letter = { 0x5 << 1,	 0x5 << 1, 0x2 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'ã'] =	{ .w = 5, .letter = { 0x5 << 1,	 0x5 << 1, 0x5 << 1,   0x5 << 1, 0xE << 1, 0x8 << 1} },
	[(unsigned char)'þ'] =	{ .w = 4, .letter = { 0x5 << 1,	 0x5 << 1, 0x6 << 1,   0x4 << 1, 0x4 << 1		} },
	[(unsigned char)'û'] =	{ .w = 6, .letter = { 0x15 << 1, 0x15 << 1, 0x15 << 1,  0x15 << 1, 0x1E << 1	} },
	[(unsigned char)'ý'] =	{ .w = 7, .letter = { 0x15 << 1, 0x15 << 1, 0x15 << 1,  0x15 << 1, 0x3E << 1, 0x20 << 1} },
	[(unsigned char)'ø'] =	{ .w = 4, .letter = { 0x1 << 1,	 0x1 << 1, 0x3 << 1,   0x5 << 1, 0x3 << 1		} },
	[(unsigned char)'à'] =	{ .w = 6, .letter = { 0x1D<< 1,	 0x15<< 1, 0x17<< 1,   0x15<< 1, 0x1D << 1		} },
	[(unsigned char)'ñ'] =	{ .w = 4, .letter = { 0x6 << 1,	 0x5 << 1, 0x6 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'Á'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x6 << 1,   0x5 << 1, 0x6 << 1		} },
	[(unsigned char)'Â'] =	{ .w = 4, .letter = { 0x6 << 1,	 0x1 << 1, 0x3 << 1,   0x5 << 1, 0x2 << 1		} },
	[(unsigned char)'×'] =	{ .w = 4, .letter = { 0x2 << 1,	 0x5 << 1, 0x3 << 1,   0x5 << 1, 0x3 << 1		} },
	[(unsigned char)'Ç'] =	{ .w = 3, .letter = { 0x0 << 1,	 0x0 << 1, 0x3 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'­'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x4 << 1, 0x7 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'Ä'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x6 << 1,   0x5 << 1, 0x6 << 1, 0x4 << 1, 0x2 << 1} },
	[(unsigned char)'Å'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x7 << 1,   0x3 << 1, 0x7 << 1		} },
	[(unsigned char)'¤'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x6 << 1,   0x3 << 1, 0x6 << 1		} },
	[(unsigned char)'Ö'] =	{ .w = 6, .letter = { 0x0 << 1,	 0x0 << 1, 0x15 << 1,  0xE << 1, 0x15 << 1	} },
	[(unsigned char)'Ú'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x3 << 1,   0x4 << 1, 0x3 << 1, 0x4 << 1, 0x3 << 1} },
	[(unsigned char)'É'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x5 << 1, 0x6 << 1		} },
	[(unsigned char)'¦'] =	{ .w = 2, .letter = { 0x1 << 1,	 0x0 << 1, 0x1 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'§'] =	{ .w = 3, .letter = { 0x5,	 0x0 << 1, 0x1 << 1,   0x1 << 1, 0x1 << 1		} },
	[(unsigned char)'Ê'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x2 << 1, 0x0 << 1,   0x5 << 1, 0x6 << 1		} },
	[(unsigned char)'Ë'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x3 << 1, 0x5 << 1		} },
	[(unsigned char)'Ì'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x6 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'Í'] =	{ .w = 6, .letter = { 0x0 << 1,	 0x0 << 1, 0x11 << 1,  0x1B << 1, 0x15 << 1	} },
	[(unsigned char)'Î'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x7 << 1, 0x5 << 1		} },
	[(unsigned char)'Ï'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x7 << 1,   0x5 << 1, 0x7 << 1		} },
	[(unsigned char)'Ð'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x7 << 1,   0x5 << 1, 0x5 << 1		} },
	[(unsigned char)'Ò'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x3 << 1,   0x5 << 1, 0x3 << 1, 0x1 << 1, 0x1 << 1} },
	[(unsigned char)'Ó'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x7 << 1,   0x1 << 1, 0x7 << 1		} },
	[(unsigned char)'Ô'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x7 << 1,   0x2 << 1, 0x2 << 1		} },
	[(unsigned char)'Õ'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x5 << 1, 0x6 << 1, 0x4 << 1, 0x2 << 1} },
	[(unsigned char)'Æ'] =	{ .w = 6, .letter = { 0x0 << 1,	 0x0 << 1, 0xE << 1,   0x15 << 1, 0xE << 1, 0x4 << 1, 0x4 << 1} },
	[(unsigned char)'È'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x2 << 1, 0x5 << 1		} },
	[(unsigned char)'Ã'] =	{ .w = 5, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x5 << 1, 0xE << 1, 0x8 << 1} },
	[(unsigned char)'Þ'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x5 << 1,   0x7 << 1, 0x4 << 1		} },
	[(unsigned char)'Û'] =	{ .w = 6, .letter = { 0x0 << 1,	 0x0 << 1, 0x15 << 1,  0x15 << 1, 0x1E << 1	} },
	[(unsigned char)'Ý'] =	{ .w = 7, .letter = { 0x0 << 1,	 0x0 << 1, 0x15 << 1,  0x15 << 1, 0x3E << 1, 0x20 << 1} },
	[(unsigned char)'Ø'] =	{ .w = 3, .letter = { 0x0 << 1,	 0x0 << 1, 0x1 << 1,   0x3 << 1, 0x3 << 1		} },
	[(unsigned char)'À'] =	{ .w = 5, .letter = { 0x0 << 1,	 0x0 << 1, 0xD << 1,   0xB << 1, 0xD << 1		} },
	[(unsigned char)'Ñ'] =	{ .w = 4, .letter = { 0x0 << 1,	 0x0 << 1, 0x7 << 1,   0x6 << 1, 0x5 << 1		} },
};

static uint8_t matrix[8];
static uint8_t position = 0;

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
	uint8_t clmask, ch;

	if (offset >= 8 || offset + alpha[letter].w <= 0)
		return alpha[letter].w;
	clmask = ((1 << alpha[letter].w) - 1);
	if (offset > 0)
		clmask <<= offset;
	else
		clmask >>= -offset;
	ch = 0;
	for (i = 0; i < 7; i++) {
		matrix[i] = (matrix[i] & ~clmask) | ch;
		ch = alpha[letter].letter[i];
		if (offset > 0)
			ch <<= offset;
		else
			ch >>= -offset;
	}
	matrix[i] = (matrix[i] & ~clmask) | ch;
	return alpha[letter].w;
}

void indicator_tick()
{
	shift_set((0xFF00 & ~(1 << position << 8)) | (matrix[position]));
	position = (position + 1) % 8;
}
