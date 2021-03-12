#include <msp430.h> 
#include "LCD.h"



int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	//initGPIO ();
	initLCD ();
	//sendData('F');
	//displayLCD("MSP430");

	checkBF();
	write('A');

	while(1);
	
	return 0;
}
