#include "iodefine.h"
#include "pccsensor.h"

volatile int r_sen, l_sen, fr_sen, fl_sen;

void init_sensor(void)
{
    PORT5.PDR.BIT.B4 = 1;
    PORT0.PDR.BIT.B5 = 1;
    PORT2.PDR.BIT.B7 = 1;
    PORTB.PDR.BIT.B5 = 1;
    PORT5.PODR.BIT.B4 = 0;
    PORT0.PODR.BIT.B5 = 0;
    PORT2.PODR.BIT.B7 = 0;
    PORTB.PODR.BIT.B5 = 0;
    PORTE.PMR.BIT.B1 = 1;
    PORT4.PMR.BIT.B4 = 1;
    PORT4.PMR.BIT.B6 = 1;
    PORT4.PMR.BIT.B2 = 1;
    MPC.PWPR.BIT.B0WI = 0;
    MPC.PWPR.BIT.PFSWE = 1;
    MPC.PE1PFS.BIT.ASEL = 1;
    MPC.P44PFS.BIT.ASEL = 1;
    MPC.P46PFS.BIT.ASEL = 1;
    MPC.P42PFS.BIT.ASEL = 1;
    MPC.PWPR.BYTE = 0x80;
    SYSTEM.PRCR.WORD = 0xA502;
    MSTP(S12AD) = 0;
    SYSTEM.PRCR.WORD = 0xA500;
    S12AD.ADCSR.BIT.CKS = 3;
    S12AD.ADCSR.BIT.ADCS = 0;
}

void int_cmt0(void)
{
    static char state = 0;
    int i;
    
    switch(state)
    {
        case 0:
            PORT0.PODR.BIT.B5 = 1;
            for(i=0; i<300; i++);
            S12AD.ADANS0.BIT.ANS0 = 0x0040;
            S12AD.ADCSR.BIT.ADST = 1;
            while(S12AD.ADCSR.BIT.ADST);
            PORT0.PODR.BIT.B5 = 0;
            r_sen = S12AD.ADDR6;
            break;
        case 1:
            PORT5.PODR.BIT.B4 = 1;
            for(i=0; i<300; i++);
            S12AD.ADANS0.BIT.ANS0 = 0x0004;
            S12AD.ADCSR.BIT.ADST = 1;
            while(S12AD.ADCSR.BIT.ADST);
            PORT5.PODR.BIT.B4 = 0;
            l_sen = S12AD.ADDR2;
            break;
        case 2:
            PORTB.PODR.BIT.B5 = 1;
            for(i=0; i<300; i++);
            S12AD.ADANS0.BIT.ANS0 = 0x0200;
            S12AD.ADCSR.BIT.ADST = 1;
            while(S12AD.ADCSR.BIT.ADST);
            PORTB.PODR.BIT.B5 = 0;
            fr_sen = S12AD.ADDR9;
            break;
        case 3:
            PORT2.PODR.BIT.B7 = 1;
            for(i=0; i<300; i++);
            S12AD.ADANS0.BIT.ANS0 = 0x0010;
            S12AD.ADCSR.BIT.ADST = 1;
            while(S12AD.ADCSR.BIT.ADST);
            PORT2.PODR.BIT.B7 = 0;
            fl_sen = S12AD.ADDR4;
            break;
    }
    state++;
    if(3<state) { state=0; } 
}
