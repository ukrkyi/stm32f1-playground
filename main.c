#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include <stm32f1xx.h>

void button_state_change_callback()
{
	if (button_is_pressed())
	{
		led_on();
		uart_send("1");
	}
	else
	{
		led_off();
		uart_send("0");
	}
}

int main(void)
{
	set_system_clock();

	led_init();
	led_off();
	encoder_init();
	uart_init();

	uart_send("Hello!\n");

	while (1) {
		;
	}
	return 0;
}
