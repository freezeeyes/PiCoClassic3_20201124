#include "iodefine.h"
#include "pccled.h"

void init_led(void)
{
    PORTB.PDR.BIT.B0 = 1;
    PORTA.PDR.BIT.B6 = 1;
    PORTA.PDR.BIT.B4 = 1;
    PORTA.PDR.BIT.B0 = 1;
}

void set_led(unsigned char data)
{
    PORTB.PODR.BIT.B0 = data & 0x01;
    PORTA.PODR.BIT.B6 = (data>>1) & 0x01;
    PORTA.PODR.BIT.B4 = (data>>2) & 0x01;
    PORTA.PODR.BIT.B0 = (data>>3) & 0x01;
}
