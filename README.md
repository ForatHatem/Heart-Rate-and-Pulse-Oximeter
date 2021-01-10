# Heart-Rate-and-Pulse-Oximeter
Project is about creating a pulse oximeter and heart rate moniter.

I am using MAX30102 as a Heart Rate and Pulse Oximeter sensor from MAXIM.

Using a MSP430FR6989 MPU to interface with the IC sensor, it shall:

 - receive raw data from the senosr using I2C protocol.
 - Apply several DSP techniques, and process the data to get the heart rate and oxygen
 - displays the data on a 2x16 LCD
 - sends the data to an HC05 Bluetooth module to be displayed on the mobile



