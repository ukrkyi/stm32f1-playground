#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include "display.h"
#include <stm32f1xx.h>
#include <stdio.h>
#include <inttypes.h>

unsigned char letters[] = "Напиши-напиши у сво╓му лист╕ "
			  "про сво╖ думки, про сво╖ бажання. "
			  "Напиши, передай ц╕ два банальн╕ рядки: "
			  "\"В мене все о'кей, в мене все нормально\". "
			  "Напиши мен╕, коли я зовс╕м один "
			  "в полон╕ годин, в чотирьох ст╕нах. "
			  "Напиши, якщо трима╓ш мо╓ фото в руках, "
			  "бо тво╓ у мене лежить на кол╕нах.\n"
			  "╤ знову день, один, два, три, чотири, п'ять - "
			  "пролет╕в ц╕лий тиждень. "
			  "Знов беззм╕стовно проведу вих╕дн╕ дн╕, "
			  "а так хот╕лося по╖хати в Карпати на лиж╕. "
			  "Де? Ну де? Ну де? Ну де? Ну де ж ви ╓, "
			  "мо╖ проблеми? - Я за вами скучаю. "
			  "Проблем нема╓ ╕ не буде, бо нема╓ тебе - "
			  "лягаю спати один ╕ кожну н╕ч чекаю.\n";

int main(void)
{
	set_system_clock();

	display_init();

	int x;

	while (1) {
		x = 0;

		for (int i = 0; i < sizeof(letters); i++) {
			x += display_putletter(letters[i]);
			for (volatile int j = 0; j < 500000; j++) ;
			if (x > 500 || letters[i] == '\n') {
				for (volatile int j = 0; j < 5000000; j++) ;
				x = 0;
				display_clear();
			}
		}

		for (volatile int j = 0; j < 5000000; j++);
		display_clear();
	}
	return 0;
}
