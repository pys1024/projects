#ifndef DELAY_H
#define DELAY_H

#include <intrins.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

void delay(unsigned int ms);

#define DELAY_3US() _nop_();
#define DELAY_4US() _nop_(); _nop_();
#define DELAY_5US() _nop_(); _nop_(); _nop_();

#endif // DELAY_H