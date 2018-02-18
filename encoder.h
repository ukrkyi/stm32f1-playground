/* (c) 2018 ukrkyi */
#ifndef ENCODER_H
#define ENCODER_H

#include <stdbool.h>

#define FULL_ROTATION 20U

extern void button_state_change_callback();
extern void encoder_step_callback(bool backward, int position);

extern bool button_is_pressed();

extern void encoder_init();

#endif // ENCODER_H
