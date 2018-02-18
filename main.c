#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include <stm32f1xx.h>
#include <stdio.h>
#include <inttypes.h>

static int position;
static char buff[100];

void button_state_change_callback()
{
	if (button_is_pressed()) {
		led_on();
		uart_send("1");
	} else {
		led_off();
		uart_send("0");
	}
}

void encoder_step_callback(bool backward, int pos)
{
	if (backward)
		uart_send("-");
	else
		uart_send("+");
	position = pos;
}

int main(void)
{
	set_system_clock();

	led_init();
	led_off();
	encoder_init();
	uart_init();

	uart_send("Hello!\r\n");

	while (1) {
		snprintf(buff, 10, "%" PRIu32 "\r\n", position);
		uart_send(buff);
		for (volatile int i = 0; i < 10000000; i++);
	}
	return 0;
}
