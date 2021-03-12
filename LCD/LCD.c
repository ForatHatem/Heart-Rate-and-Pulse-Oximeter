#include <msp430.h>
#include "LCD.h"

/*#define FACLK       32768
#define DELAY_50MS  ((50*FACLK)/1000) //t = count * TACLK -> count = t * FACLK
#define DELAY_10MS  ((10*FACLK)/1000)
//#define DELAY_200US ((200*FACLK)/(1000*1000))
#define DELAY_200US 7*/


//Fsmclk = 1MHz t = count* Tsmclk   count = t * Fsmclk
//#define LCD_STARTUP    40000//> 40ms count = 40ms * 1000 khz
//#define LCD_F_INSTR    40// > 37us count = 37 us * 1
//#define LCD_S_INSTR    1520//> 1.52 ms  count = 1.52 ms * 1000 Khz

/*#define DELAY_40MS  40000
#define DELAY_5MS   5000// 5ms * 1000 Khz
#define DELAY_160US 160// 160*/

#define DELAY_40MS  40000
#define DELAY_40US  40// 40
#define DELAY_2MS   2000//2000 * 1
#define DELAY_1US   1
#define DELAY_80US  80


//#define CHK             1
//#define NO_CHK          0

/*
#define DATA    1
#define INST    0
#define WRITE
#define READ
 */

//for debugging purposes
unsigned int E_Pin = 0;
unsigned int RS_Pin = 0;
unsigned int RW_Pin = 0;
unsigned int BF = 0;

enum CommandLCD
{
    FunctionSet = 0x38,// 0011 1100
    DisplayON = 0x0C,// 0000 1100
    EntryModeSet = 0x06,//0000 0110
    ClearDisplay = 0x01,// 0000 0001
    ReturnCursorHome = 0x02//
};

void initGPIO (void)
{
    P9DIR = 0xFF;//output
    P4DIR = 0xFF;//output

    P4OUT = 0x00;
    P9OUT = 0x00;
}

void initTMR (void)
{
    TA0CTL |= (MC__CONTINUOUS | TASSEL__SMCLK);
    TA0CTL &= ~TAIFG;
}

void configClock (void)
{
    CSCTL0 = CSKEY;
    CSCTL1 = 0x0000;//DCO 1MHz
    CSCTL2 |= (SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK);
}

/*void delay (unsigned int tmr)
{
    TA0CCR0 = tmr;
    TA0CTL |= ;//reset TA0R
    while(!(TA0CTL & TAIFG));
    TA0CTL &= ~TAIFG;
}*/

void delay(unsigned int tmr)
{
    TA0R = 0;
    while(TA0R < tmr);
    TA0CTL &= ~TAIFG;
}

void checkBF (void)
{
    unsigned int BF;

    RS(0);//Inst
    RW(1);//Read

    do
    {
        E(1);
        delay(DELAY_1US);
        E(0);
        TA0R = 0;
        while(TA0R < DELAY_80US)
        {
            if(P4OUT & BIT3)//BF = 1
                BF = 1;
            else
                BF = 0;
        }
    }while(BF);

}


/*void initLCD(void)
{
    initGPIO ();
    configClock ();
    initTMR ();

    E(0);
    delay(LCD_STARTUP);//40 ms
    sendCommand(FunctionSet);
    delay(LCD_F_INSTR);//37 us
    sendCommand(FunctionSet);
    delay(LCD_F_INSTR);//37us
    sendCommand(FunctionSet);
    delay(LCD_F_INSTR);

    clearBF();
    sendCommand(DisplayON);
    delay(LCD_F_INSTR);

    clearBF();
    sendCommand(EntryModeSet);
    delay(LCD_F_INSTR);

    clearBF();
    sendCommand(ClearDisplay);
    delay(LCD_S_INSTR);

    clearBF();
    sendCommand(EntryModeSet);

    clearBF();
    sendCommand(ReturnCursorHome);
    delay(LCD_S_INSTR);

}*/

void initLCD (void)
{
    initGPIO ();
    configClock ();
    initTMR ();

    E(0);
    delay(DELAY_40MS);
    command (0x38);
    delay(DELAY_40US);
    command (0x38);
    delay(DELAY_40US);

    checkBF();
    command (0x0C);//Display ON
    delay(DELAY_40US);

    checkBF();
    command(0x01);//Clear Displayt
    delay(DELAY_2MS);

    checkBF();
    command(0x07);//Entry Mode Set
    delay(DELAY_2MS);
}

void command (char ch)
{
    outputPORT(ch);

    RS(0);//Ins
    RW(0);//Write

    E(1);
    delay(DELAY_1US);//PW >= 450 ns
    E(0);
    delay(DELAY_1US);
}

void write (char ch)
{
    outputPORT(ch);

    RS(1);//Data
    RW(0);//Write

    E(1);
    delay(DELAY_1US);
    E(0);
    delay(DELAY_1US);

    delay(DELAY_40US);
}

void outputPORT (char ch)
{
    P9OUT = (ch & 0x7F);
    ch = ch >> 4;
    P4OUT |= (ch & 0x08);
}

/*void sendData (char ch)//write
{
    //clearBF ();//clear BF

    P9OUT = (ch & 0x7F);
    ch = ch >> 4;
    P4OUT |= (ch & 0x08);

    RS(1);
    RW(0);
    E(1);
    delay(1);//t = count * (1/Fsmclk) = 1 / 1 us
    E(0);

    //delay(LCD_F_INSTR);
    delay(DELAY_160US);
}*/

/*void sendCommand (char ch)//command
{
    P9OUT = (ch & 0x7F);
    ch = ch >> 4;
    P4OUT |= (ch & 0x08);

    RS(0);
    RW(0);
    E(1);
    delay(1);
    E(0);
}*/

void RS(int i)
{
    if(i == 1)
    {
        P4OUT |= BIT0;
        RS_Pin = 1;
    }
    else
    {
        P4OUT &= ~BIT0;
        RS_Pin = 0;
    }
}

void RW(int i)
{
    if(i == 1)
    {
        P4OUT |= BIT1;
        RW_Pin = 1;
    }
    else
    {
        P4OUT &= ~BIT1;
        RW_Pin = 0;
    }
}

void E (int i)
{
    if(i == 1)
    {
        P4OUT |= BIT2;
        E_Pin = 1;
    }
    else
    {
        P4OUT &= ~BIT2;
        E_Pin = 0;
    }
}

void displayLCD(char * str)
{
    while(*str != '\0')
    {
        sendData(*str);
        str++;
    }
}
