QUADCOPTER
==========

A quadcopter controler on a Raspberry PI

This project is the development of a code to control a quadcopter
with a Raspberry Pi.

The main idea is to use the RPI as much as possible. The only external
devices needed are a MPU6050 for attitude measurment and a Wifi dongle.
The quadcopter is remotely controlled via wifi and an Android App.


The Raspberry PI hosts:
- PID controller 
- ESC signal generator (ServoBlaster)
- UDP server to get command from an android device
- Communication with the MPU6050 through I2C.

The repository has several folders, that I initially made to code and test 
the different functions of the controller. The entire controller is in the 
PILOT folder.

This proect is greatly inspired from :
- https://github.com/richardghirst/PiBits 
for ServoBlaster and the MPU6050 usage

- https://github.com/big5824/Picopter
for the Timing and the code structure

- https://code.google.com/p/owenquad/
for the Android app

Many thanks to these people.
