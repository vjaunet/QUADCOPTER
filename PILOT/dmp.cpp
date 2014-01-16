/*

  DMP class
  author : vincent jaunet
  date : 10-01-2013

  Description :
  The DMP class is mainly a wrapper to the MPU6050
  one from github/PiBits and Jeff Rowberg <jeff@rowberg.net>

  It defines the main functions to :
  -set up the I2C communication through I2Cdev
  -Initialize the measurements and retrieve Offset values
  -Get the attitude of the drone

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
  YAW = 0;
  PITCH = 1;
  ROLL = 2;
  DIM = 3;

  for (int i=0;i<DIM;i++) lastval[i]=10;
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

  for (int i=0;i<DIM;i++) {
    offset[i] = ypr[i];
  }
  printf("IMU init done; offset values are :\n");
  printf("yaw = %f, pitch = %f, roll = %f\n",
	 offset[YAW]*180/M_PI, offset[PITCH]*180/M_PI,
	 offset[ROLL]*180/M_PI);
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
    for (int i=0;i<DIM;i++) ypr[i]-=offset[i];

    mpu.dmpGetGyro(g, fifoBuffer);
    gyro[YAW]   = (float)(g[2])/131.0f;
    gyro[PITCH] = (float)(g[1])/131.0f;
    gyro[ROLL]  = (float)(g[0])/131.0f;
  }
}
