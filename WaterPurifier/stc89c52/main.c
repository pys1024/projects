#include "reg52.h"
#include "delay.h"
#include "i2c.h"


sbit beep = P2^5;
/*
sbit pump = P3^6;

sbit valve1 = P1^6;
sbit valve2 = P1^5;
sbit valve3 = P1^4;
sbit valve6 = P1^3;
sbit valve5 = P1^2;
sbit valve4 = P1^1;

sbit sw1 = P0^3;
sbit sw2 = P0^2;
sbit sw3 = P0^1;
*/


void init() {
    P0 = 0x00;
    P1 = 0x00;
    P2 = 0x00;
    P3 = 0x00;
}

void uart_init()
{
    TMOD = 0x20;
    TH1 = 0xFD;
    TL1= 0xFD;
    TR1 = 1;
    REN = 1;
    SM0 = 0;
    SM1 = 1;
 
    ES=1;           //设置接收发送中断
	EA=1;
 
 
}
void uart_send_byte(unsigned char byte)
{
	SBUF = byte;
	while(TI == 0);
	TI = 0;
}

void gpio_write(uint8_t _port, uint8_t _bit, uint8_t value) {
    switch (_port) {
        case 0:
            if (value) P0 |= (1 << _bit);
            else P0 &= ~(1 << _bit);
            break;
        case 1:
            if (value) P1 |= (1 << _bit);
            else P1 &= ~(1 << _bit);
            break;
        case 2:
            if (value) P2 |= (1 << _bit);
            else P2 &= ~(1 << _bit);
            break;
        case 3:
            if (value) P3 |= (1 << _bit);
            else P3 &= ~(1 << _bit);
            break;

        case 8:
            P0 = value;
            break;
        case 9:
            P1 = value;
            break;
        case 10:
            P2 = value;
            break;
        case 11:
            P3 = value;
            break;
    }
}

uint8_t gpio_read(uint8_t _port, uint8_t _bit) {
    switch (_port) {
        case 0:
            return (P0 >> _bit) & 0x01;
        case 1:
            return (P1 >> _bit) & 0x01;
        case 2:
            return (P2 >> _bit) & 0x01;
        case 3:
            return (P3 >> _bit) & 0x01;

        case 8:
            return P0;
        case 9:
            return P1;
        case 10:
            return P2;
        case 11:
            return P3;
    }
    return 0;
}

void main() {
    uint8_t addr = 0;
    uint8_t value = 0;

    init();
    i2c_init();
    uart_init();

    delay(1000);
    uart_send_byte(0xA5); // send hello message to PC

    while(1) {
       if (i2c_slave_start(&addr)) {

            if (addr & 0x01) { // host want to read
                value = gpio_read((addr >> 4) & 0x0F, (addr >> 1) & 0x07);
                i2c_slave_write(value);
            } else {
                value = i2c_slave_read();
                gpio_write((addr >> 4) & 0x0F, (addr >> 1) & 0x07, value);
            }

            // uart_send_byte(addr);
            // uart_send_byte(value);
       }

       i2c_slave_stop(); // wait for master to send stop signal
    }
}

void uart_rx_isr() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;
	}
}
