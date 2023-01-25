# Arduino-codes
//----------------------------------------------

TachoHighPrec: 
High-precision tachometer code for Arduino Nano RP2040. 
It measures rotation speed from 30RPM to 30000RPM. 
We count time-lapse for 'nPulses' pulses, then average the 
period for each revolution to obtain a high precision RPM value. 

//----------------------------------------------

IMUSample: 
This code is a basic demo for accelerometer & gyro readings from Adafruit
LSM6DSOX sensor
1. Install Arduino Mbed OS Nano Boards by Arduino
2. Choose Arduino Nano RP2040 board
3. Install libraries for acceleromter and gyro: 
  The 1st method: Install "Arduino_LSM6DSOX" lib
  This lib hellps us reading Arudino Nano onboard accelerometer and gyro. 
  However, the maximum acceleration is limited to 4g. 
  The 2nd method: Install "Adafruit LSM6DS" lib
  This lib provides more basic functions to read onboard accelerometer and gyro. 
  Therefore the maximum acceleration can be higher than 4g. 
4. In this code, we are using "Adafruit LSM6DS" lib
