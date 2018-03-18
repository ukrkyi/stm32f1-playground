#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include "indicator.h"
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

	systick_init(9000, true); // 1 tick per ms

	indicator_init();

	int offset = 0, position;

	while (1) {
		position = 8;
		for (int i = 0; i < sizeof(letters); i++)
			position += indicator_putletter(letters[i], position - offset);

		offset = (offset + 1) % position;
		for (volatile int i = 0; i < 500000; i++);
	}
	return 0;
}
