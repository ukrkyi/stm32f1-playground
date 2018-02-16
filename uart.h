/* (c) 2018 ukrkyi */
#ifndef UART_H
#define UART_H

#include <stdbool.h>
#include <stdint.h>

extern void uart_init();
extern bool uart_send(char * data);

extern void uart_byte_received_callback(char, uint16_t);

#endif // UART_H
