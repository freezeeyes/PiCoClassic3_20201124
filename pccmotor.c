#include "iodefine.h"
#include "pccmotor.h"

volatile float v, a;
volatile unsigned int step_l, step_r;
volatile char attitude_control;
extern volatile int r_sen, l_sen;

void init_motor(void)
{
    PORT1.PODR.BIT.B5 = 0;
    PORT1.PDR.BIT.B5 = 1;
    PORTC.PDR.BIT.B5 = 1;
    PORTC.PDR.BIT.B6 = 1;
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;
    MPC.P17PFS.BIT.PSEL = 1;
    MPC.PE2PFS.BIT.PSEL = 1;
    MPC.PWPR.BYTE = 0x80;
    PORT1.PMR.BIT.B7 = 1;
    PORTE.PMR.BIT.B2 = 1;
    
    SYSTEM.PRCR.WORD = 0xA502;
    MSTP(MTU) = 0;
    SYSTEM.PRCR.WORD = 0xA500;
    MTU.TSTR.BYTE = 0;
    
    MTU3.TCR.BIT.CCLR = 1;
    MTU3.TCR.BIT.TPSC = 2;
    MTU3.TMDR.BIT.MD = 2;
    MTU3.TIORH.BIT.IOA = 2;
    MTU3.TIORH.BIT.IOB = 1;
    MTU3.TGRA = 6297;
    MTU3.TGRB = 50;
    MTU3.TGRC = 6297;
    MTU3.TMDR.BIT.BFA = 1;
    MTU3.TIER.BIT.TGIEB = 1;
    IEN(MTU3,TGIB3) = 1;
    IPR(MTU3,TGIB3) = 13;
    IR(MTU3,TGIB3) = 0;
    
    MTU.TOER.BIT.OE4A = 1;
    MTU4.TCR.BIT.CCLR = 1;
    MTU4.TCR.BIT.TPSC = 2;
    MTU4.TMDR.BIT.MD = 2;
    MTU4.TIORH.BIT.IOA = 2;
    MTU4.TIORH.BIT.IOB = 1;
    MTU4.TGRA = 6297;
    MTU4.TGRB = 50;
    MTU4.TGRC = 6297;
    MTU4.TMDR.BIT.BFA = 1;
    MTU4.TIER.BIT.TGIEB = 1;
    IEN(MTU4,TGIB4) = 1;
    IPR(MTU4,TGIB4) = 12;
    IR(MTU4,TGIB4) = 0;
    
    MTU3.TCNT = 0;
    MTU4.TCNT = 0;
}

void int_mot_r(void)
{
    step_r++;
}

void int_mot_l(void)
{
    step_l++;
}

void int_cmt1(void)
{
    int sen_diff, sen_diff_r, sen_diff_l, scale;
    float p;
    
    v = a + v;
    if(attitude_control)
    {
        scale = 1;
        if((r_wall_ref + r_sen_ref) / 2 < r_sen)
        {
            sen_diff_r = r_sen - r_sen_ref;
        }
        else
        {
            sen_diff_r = 0;
            scale = 2;
        }
        if((l_wall_ref + l_sen_ref) / 2 < l_sen)
        {
            sen_diff_l = l_sen_ref - l_sen;
        }
        else
        {
            sen_diff_l = 0;
            scale = 2;
        }
        sen_diff = (sen_diff_l + sen_diff_r) * scale;
        p = (float)sen_diff * KP;
    }
    else
    {
        p = 0;
    }
    
    MTU3.TGRC = 3000000 / ((v+p) / 0.377);
    MTU4.TGRC = 3000000 / ((v-p) / 0.377);
}

void kasoku(float accel)
{
    int i;
    PORTC.PODR.BIT.B5 = 0;
    PORTC.PODR.BIT.B6 = 0;
    PORT1.PODR.BIT.B5 = 1;
    for(i=0; i<1000; i++);
    v = 180;
    a = accel;
    step_l = step_r = 0;
    MTU.TSTR.BIT.CST3 = 1;
    MTU.TSTR.BIT.CST4 = 1;
    while((step_l+step_r) < (477/2)*2)
    {
        if(v<300)
        {
            a = accel;
        }
        else
        {
            a = 0;
        }
    }
    MTU.TSTR.BIT.CST3 = 0;
    MTU.TSTR.BIT.CST4 = 0;
}

void gensoku(float accel)
{
    v = 300;
    a = accel * -1;
    step_l = step_r = 0;
    MTU.TSTR.BIT.CST3 = 1;
    MTU.TSTR.BIT.CST4 = 1;
    while((step_l+step_r) < (477/2)*2)
    {
        if(180<v)
        {
            a = accel * -1;
        }
        else
        {
            a = 0;
        }
    }
    MTU.TSTR.BIT.CST3 = 0;
    MTU.TSTR.BIT.CST4 = 0;
}

void step(void)
{
    v = 300;
    a = 0;
    step_l = step_r = 0;
    MTU.TSTR.BIT.CST3 = 1;
    MTU.TSTR.BIT.CST4 = 1;
    while((step_l+step_r) < 477*2);
    MTU.TSTR.BIT.CST3 = 0;
    MTU.TSTR.BIT.CST4 = 0;
}

void rotate180(void)
{
    int i;
    a = 0;
    
    PORTC.PODR.BIT.B5 = 0;
    PORTC.PODR.BIT.B6 = 0;
    PORT1.PODR.BIT.B5 = 1;
    for(i=0; i<1000; i++);
    v = 180;
    step_l = step_r = 0;
    MTU.TSTR.BIT.CST3 = 1;
    MTU.TSTR.BIT.CST4 = 1;
    while((step_l+step_r) < 146*4);
    MTU.TSTR.BIT.CST3 = 0;
    MTU.TSTR.BIT.CST4 = 0;
    for(i=0; i<1000; i++);
}
