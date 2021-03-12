#ifndef LCD_H_
#define LCD_H_

void initGPIO (void);//for lcd
void initTMR (void);
void configClock (void);//DCO 1MHz

void initLCD (void);
void displayLCD (char*);
//void sendCommand (char);
//void sendData (char);
void command (char);
void write (char);
void outputPORT (char);

void checkBF (void);

void E (int);//E enable LCD
void RS (int);//DATA or INS
void RW (int);

void delay (unsigned int);//ms

#endif /* LCD_H_ */
