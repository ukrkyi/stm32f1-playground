/* (c) 2018 ukrkyi */
#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

extern void set_system_clock(void);
extern bool systick_init(uint32_t ticks, bool div8);

#endif // SYSTEM_H
