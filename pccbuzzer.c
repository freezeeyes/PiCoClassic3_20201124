#include "iodefine.h"
#include "pccbuzzer.h"

void init_buzzer(void)
{
    PORTB.PODR.BIT.B3 = 1;
    PORTB.PDR.BIT.B3 = 1;
    PORTB.PMR.BIT.B3 = 0;
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;
    MPC.PB3PFS.BIT.PSEL = 1;
    MPC.PWPR.BYTE = 0x80;
    SYSTEM.PRCR.WORD = 0xA502;
    MSTP(MTU) = 0;
    SYSTEM.PRCR.WORD = 0xA500;
    MTU.TSTR.BYTE = 0;
    MTU0.TCR.BIT.CCLR = 2;
    MTU0.TCR.BIT.TPSC = 1;
    MTU0.TMDR.BIT.MD = 2;
    MTU0.TIORH.BIT.IOA = 5;
    MTU0.TIORH.BIT.IOB = 2;
    MTU0.TGRA = (13636 - 1);
    MTU0.TGRB = (27272 - 1);
    MTU0.TCNT = 0;
}

void buzzer_wait(void)
{
    int i;
    buzzer_on();
    for(i=0; i<0xFFFFF; i++);
    buzzer_off();
}

void buzzer_on(void)
{
    MTU0.TCNT = 0;
    PORTB.PMR.BIT.B3 = 1;
    MTU.TSTR.BIT.CST0 = 1;
}

void buzzer_off(void)
{
    PORTB.PMR.BIT.B3 = 0;
    MTU.TSTR.BIT.CST0 = 0;
}
