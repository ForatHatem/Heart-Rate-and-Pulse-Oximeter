/*
        ________________                    ________________
        |               | SCL/P1.7          |               |
        |               |-------------------|               |
        |               |                   |               |
        |   msp430      |                   |    max30102   |
        |               | SDA/P1.6          |               |
        |               |-------------------|               |
        |_______________|                   |_______________|


*/
#include <msp430.h> 
#include "max30102.h"
#include "i2c.h"
#include "config.h"

uint32_t aun_ir_buffer[100]; //infrared LED sensor data
uint32_t aun_red_buffer[100];  //red LED sensor data
int32_t n_ir_buffer_length; //data length
int32_t n_spo2;  //SPO2 value
int8_t ch_spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t n_heart_rate; //heart rate value
int8_t  ch_hr_valid;  //indicator to show if the heart rate calculation is valid
uint8_t uch_dummy;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

    configClock ();
    initI2C ();
    initTMR ();

    __bis_SR_register(GIE);

    maxim_max30102_reset ();

    delay(10);//10 ms

    maxim_max30102_init ();

    n_ir_buffer_length = 100;

    int i = 0;
    for(i = 0; i < n_ir_buffer_length;i++)
    {
        delay(10);
        maxim_max30102_read_fifo((aun_red_buffer + i),(aun_ir_buffer + i));
    }

    //calc. from algorithm
    maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);

    while(1)
    {
        //delete oldest 25 sample (0 - 24), and shift the samples to the left
        i = 25;
        for(i = 25; i < 100;i++)
        {
            aun_ir_buffer[i - 25] = aun_ir_buffer[i];
            aun_red_buffer[i - 25] = aun_red_buffer [i];
        }

        //add the latest 25 samples
        i = 75;
        for(i = 75; i < 100; i++)
        {
            delay(10);
            maxim_max30102_read_fifo((aun_red_buffer + i),(aun_ir_buffer + i));
        }

        //calc. hr and spo2
        maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_spo2, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid);
    }

	//while(1);
	
	return 0;
}
