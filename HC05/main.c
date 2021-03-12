#include <msp430.h> 

/*   ______ TX-4.2   RX_______
    |      |-------->|        |
    | MSP  |         |  HC-05 |
    |      |<--------|        |
    |______|RX-4.3   TX_______|
*/

#define BR_VALUE  26
#define MOD_VALUE 0xB601

#define CONFIG_BR_REG  (UCA0BRW = BR_VALUE)
#define CONFIG_MOD_REG (UCA0MCTLW = MOD_VALUE)

#define UART_TX_BUF UCA0TXBUF
#define UART_RX_BUF UCA0RXBUF

#define DELAY_1SEC  32768//count = 1 * 32768
#define DELAY_150MS 4915//count = 150 ms * 32.768 KHz

void configUART (void);
void configClock (void);
void sendData (char ch);
void configTMR (void);
void delay (unsigned int);
void sendString (char*);

char strArry [6][10] = {"SpO2: ","88","%","\n","BPM: ","320"};


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT4;//output
    P1OUT &= ~BIT4;

    configClock ();
    configUART ();//eUSCI A0
    configTMR ();

    int i = 0;

    /*while(1)
    {
        sendString(strArry[i]);//strArry is char** strArry[i] is char*
        i++;
        if(i > 2)
            i = 0;
        delay (DELAY_1SEC);
        sendString("\n");
    }*/

    while(1)
    {
        sendString(strArry[i]);
        i++;
        if(i>5)
        {
            i = 0;
            sendString("\n\n");
        }
        delay(1000);
        //sendString("\n");
    }

    /*while(1)
    {
        if(UART_RX_BUF == 'B')
            P1OUT |= BIT4;
        else
            P1OUT &= ~BIT4;
    }*/


	
	return 0;
}

void configUART (void)
{
    //Config UART module UCA0
    UCA0CTLW0 |= UCSWRST;

    UCA0CTLW0 &= ~ (UCPEN | UCMSB | UC7BIT | UCSPB | UCSYNC);
    //No Parity, LSB, 8 bits, One Stop Bit, Ascyc.
    UCA0CTLW0 |= UCSSEL__SMCLK;


    //Baud Rate 9600 clock signal from DCO SMCLK 4MHz
    CONFIG_MOD_REG;
    CONFIG_BR_REG;

    UCA0CTLW0 &= ~UCSWRST;//UART is set for operation

    //Config Pins P4.3 RX   and P4.2 TX
    P4SEL0 |= (BIT2 | BIT3);
    P4SEL1 &= ~(BIT2 | BIT3);
}

void configClock (void)
{
    CSCTL0 = CSKEY;//password in order to modify clock signal
    CSCTL1 = DCOFSEL_3;//DCO 4Mhz
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;//ACLK -> LFXTCLK (32.768KHz) SMCLK & MCLK -> DCOCLK
    CSCTL3 = 0x0000;//DIVS DIVM DIVA are 0
    CSCTL4 &= ~LFXTOFF;

    //PJSEL0 |= (BIT4 | BIT5);
    PJSEL0 |= BIT4;
    PJSEL1 &= ~BIT4;
}



void sendData (char ch)
{
    UCA0TXBUF = ch;
}

void configTMR (void)
{
    TA0CTL |= (MC__CONTINOUS | TASSEL__ACLK);
}

/*void delay (unsigned int tmr)
{
    TA0R = 0;
    while(TA0R < tmr);
}*/

void delay(unsigned int tmr)
{
    unsigned int count = 32768/1000;
    count = count * tmr;
    TA0R = 0;
    while(TA0R < count);
}

void sendString (char*str)
{
    while(*str != '\0')
    {
        UCA0TXBUF = *str;
        str++;
        //delay(DELAY_150MS);
        delay(50);
    }
}
