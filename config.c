#include <msp430.h>
#include "config.h"

void configClock (void)
{
    CSCTL0 = CSKEY;
    CSCTL1 = 0x0000;//DCO 1MHz
    CSCTL2 |= (SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK);
}

void initTMR (void)
{
    TA0CTL |= (MC__CONTINOUS | TASSEL__ACLK);
}

void delay (unsigned int tmr)
{
    //t = count * Taclk
    //count = t*fACLK
    unsigned int count = 32768/1000;
    count = count * tmr;//t in ms
    TA0R = 0;
    while(TA0R < count);
}


