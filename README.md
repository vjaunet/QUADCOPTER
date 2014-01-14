QUADCOPTER
==========

A quadcopter controller on a Raspberry PI
------------------------------------------

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

This project is greatly inspired and using source code from :
- https://github.com/richardghirst/PiBits
for ServoBlaster that I use to control the ESCs and the MPU6050 Digital motion processing.

- https://github.com/big5824/Picopter
for the Timing and the code structure

- https://code.google.com/p/owenquad/
for the Android app

Many thanks to these people.


Hardware
--------

This projects includes :
- 4 brushless motors (TURNIGY 2204-14T 19g Outrunner)
- 4 Electronic Speed Controllers (Turnigy Multistar 10 Amp)
- 1 LiPo 3s 3700 mAh battery
- 1 sparkfun MPU6050 breakout board
- 1 QuadCopter frame
- 1 Android device for remote

Wiring
------

MPU6050 :
-VDD -> 3.3V
-GND -> GND
-SDA -> SDA
-SCL -> SCL
-VIO -> 3.3V


ESCs:
-GND -> GND
-Signal Servo 0 -> pin 4
-Signal Servo 1 -> pin 23
-Signal Servo 2 -> pin 24
-Signal Servo 3 -> pin 25

Note that the use of this code requires some modifications of the Servoblaster configuration.







