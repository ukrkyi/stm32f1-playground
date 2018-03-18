/* (c) 2018 ukrkyi */
#ifndef INDICATOR_H
#define INDICATOR_H

extern void indicator_init();
extern void indicator_setpoint(int x, int y);
extern void indicator_clearpoint(int x, int y);
extern void indicator_clear();
extern int indicator_putletter(unsigned char letter, int offset);

#endif // INDICATOR_H
