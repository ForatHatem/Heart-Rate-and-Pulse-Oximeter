# Heart-Rate-and-Pulse-Oximeter
Project is about creating a pulse oximeter and heart rate moniter.

I am using MAX30102 as a Heart Rate and Pulse Oximeter sensor from MAXIM.

Using a MSP430FR6989 MPU to interface with the IC sensor, it shall:

 - receive raw data from the senosr using I2C protocol.
 - processes the received data from ADC values to HR and SPO2 values
 - displays the data to a 2x16 LCD
 - sends the data to an HC05 Bluetooth module to be displayed on the mobile
 - stores the data in an SD card.



