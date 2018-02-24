#ifndef LED_H
#define LED_H

extern void led_init();
extern void led_on();
extern void led_off();
extern void led_toggle();

extern void led_rgb_init();
extern void led_rgb_setcolor(unsigned r, unsigned g, unsigned b);

#endif // LED_H
