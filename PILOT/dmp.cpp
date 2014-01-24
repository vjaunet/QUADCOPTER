/*
  ---------------------------
  DMP class
  author : vincent jaunet
  date : 10-01-2013
  ---------------------------

  Description :
  The DMP class is mainly a wrapper to the MPU6050
  one from github/PiBits and Jeff Rowberg <jeff@rowberg.net>

  It defines the main functions to :
  -set up the I2C communication through I2Cdev
  -Initialize the measurements and retrieve Offset values
  -Get the attitude of the drone

  Copyright (c) <2014> <Vincent Jaunet>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "dmp.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

MPU6050 mpu;

// MPU control/status vars
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation
                          //(0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

int32_t g[3];              // [x, y, z]            gyro vector
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

DMP imu;

DMP::DMP()
{
  dmpReady=true;
  initialized = false;
  for (int i=0;i<DIM;i++){
    lastval[i]=10;
    offset[i]=0;
  }

}


//---------------------------
//         mpu setup
//---------------------------
void DMP::set_com() {
    // initialize device
    printf("Initializing I2C devices...\n");
    mpu.initialize();

    // verify connection
    printf("Testing device connections...\n");
    printf(mpu.testConnection() ? "MPU6050 connection successful\n" : "MPU6050 connection failed\n");

    // load and configure the DMP
    printf("Initializing DMP...\n");
    devStatus = mpu.dmpInitialize();

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        printf("Enabling DMP...\n");
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        //Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        //attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        printf("DMP ready!\n");
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        printf("DMP Initialization failed (code %d)\n", devStatus);
    }
}

void DMP::initialize(){

  //This routine waits for the yaw angle to stop
  //drifting

  if (!dmpReady) return;

  printf("Initializing IMU...\n");

  for (int n=1;n<3500;n++) {

    // wait for FIFO count > 42 bits
    do {
      fifoCount = mpu.getFIFOCount();
    }while (fifoCount<42);

    if (fifoCount >= 1024) {
      // reset so we can continue cleanly
      mpu.resetFIFO();
      printf("FIFO overflow!\n");

      // otherwise, check for DMP data ready interrupt
      //(this should happen frequently)
    } else {
      //read packet from fifo
      mpu.getFIFOBytes(fifoBuffer, packetSize);

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

       // printf("yaw = %f, pitch = %f, roll = %f\n",
       // 	     ypr[YAW]*180/M_PI, ypr[PITCH]*180/M_PI,
       // 	      ypr[ROLL]*180/M_PI);
    }
  }

  //no need to offset the Pitch and Roll
  //the accelerometer gives the gravity direction to the DMP.
  offset[0] = ypr[0];
  printf("IMU init done; offset values are :\n");
  printf("yaw = %f, pitch = %f, roll = %f\n\n",
	 offset[YAW]*180/M_PI, offset[PITCH]*180/M_PI,
	 offset[ROLL]*180/M_PI);
  initialized = true;
}


void DMP::getAttitude()
{
  if (!dmpReady) return;

  // wait for FIFO count > 42 bits
  do {
  fifoCount = mpu.getFIFOCount();
  }while (fifoCount<42);

  if (fifoCount == 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    printf("FIFO overflow!\n");

    // otherwise, check for DMP data ready interrupt
    //(this should happen frequently)
  } else  {
    //read packet from fifo
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    //removing offset values
    for (int i=0;i<DIM;i++){
      ypr[i]-=offset[i];
      ypr[i]*=180/M_PI;
    }

    mpu.dmpGetGyro(g, fifoBuffer);
    for (int i=0;i<DIM;i++){
      gyro[i]   = (float)(g[i])/131.0;
   }
  }
}
