#include "led.h"
#include "encoder.h"
#include "interrupt.h"
#include "uart.h"
#include "system.h"
#include <stm32f1xx.h>
#include <stdio.h>
#include <inttypes.h>

static int max = 100;
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
	if (backward) {
		if (max > 0)
			max--;
	} else if (max < 100)
		max++;
}

int main(void)
{
	int i = 0, j = 100;
	bool up = false;

	set_system_clock();

	led_init();
	led_off();
	led_rgb_init();
	led_rgb_setcolor(100, 100, 100);
	encoder_init();
	uart_init();

	uart_send("Hello!\r\n");

	while (1) {
		switch (i) {
		case 0: led_rgb_setcolor(j, max, max); break;
		case 1: led_rgb_setcolor(max, j, max); break;
		case 2: led_rgb_setcolor(max, max, j); break;
		case 3: led_rgb_setcolor(j, j, max); break;
		case 4: led_rgb_setcolor(j, max, j); break;
		case 5: led_rgb_setcolor(max, j, j); break;
		case 6: led_rgb_setcolor(j, j, j); break;
		}
		if (up) {
			j++;
			if (j >= max) {
				up = false;
				i = (i + 1) % 7;
			}
		} else {
			j--;
			if (j == 0)
				up = true;
		}
		for (volatile k = 0; k < 200000; k++);
	}
	return 0;
}
