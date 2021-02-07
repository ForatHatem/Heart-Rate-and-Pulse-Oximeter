#include <msp430.h>
#include <stdint.h>
#include "i2c.h"




void initI2C()
{
    //config P1.6 SDA P1.7 SCL
    P1SEL1 &= ~ (BIT6 | BIT7);
    P1SEL0 |= (BIT6 | BIT7);

    UCB0CTLW0 = UCSWRST;                      // Enable SW reset
    UCB0CTLW0 |= UCMODE_3 | UCMST | UCSSEL__SMCLK | UCSYNC; // I2C master mode, SMCLK
    UCB0BRW = 10;//3;//10;                            // fSCL = SMCLK/10 = ~100kHz
    //UCB0I2CSA = SLAVE_ADDR;                   // Slave Address
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
    UCB0IE |= UCNACKIE;//enable NACK ISR (TX and RX?)
}

void i2c_start (uint8_t dev_addr,unsigned int RW)
{
    while(UCB0STAT & UCBBUSY);//check if SDA and SCL are idle

    UCB0I2CSA = dev_addr;

    if(RW == READ){UCB0CTLW0 &= ~UCTR;}
    else{UCB0CTLW0 |= UCTR;}

    UCB0CTLW0 |= UCTXSTT;

    while (UCB0CTLW0 & UCTXSTT);//wait till the whole address has been sent and ACK'ed
}

void i2c_stop (void)
{
    UCB0CTLW0 |= UCTXSTP;//stop
    while(UCB0CTLW0 & UCTXSTP);//wait for a stop to happen
}

void i2c_repeated_start(uint8_t dev_addr, unsigned int RW)
{
    UCB0I2CSA = dev_addr;

    if(RW == READ){UCB0CTLW0 &= ~UCTR;}
    else{UCB0CTLW0 |= UCTR;}

    UCB0CTLW0 |= UCTXSTT;

    while (UCB0CTLW0 & UCTXSTT);//wait till the whole address has been sent and ACK'ed
}

void i2c_write (uint8_t data)
{
    UCB0TXBUF = data;

    while(UCB0CTLW0 & UCTXIFG0);//1 means data is sent completely
}

void i2c_read (uint8_t * led,unsigned int RxByteCtr)
{
    int i = 0;
    for(i = 0; i < RxByteCtr;i++)
    {
        while(!(UCB0IFG & UCRXIFG0));//make sure rx buffer got data

        if(i == RxByteCtr - 1)
            STOP_I2C;

        led[i] = UCB0RXBUF;
    }

    while(UCB0CTLW0 & UCTXSTP);//wait for a stop to happen
}
