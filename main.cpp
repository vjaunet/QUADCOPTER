#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "MPU6050s/mpu6050_dmp.h"

#include "global.h"

//-------------------------------------
//--------- Main-----------------------

int  SetServo(int ServoId, int ServoPosition)
{
  if(servo==NULL) return(0);
  fprintf(servo, "%d=%d\n",ServoId,ServoPosition);
  fflush(servo);
  return(1);
}


int main(int argc, char *argv[])
{
  printf("Hello world!\n");
  printf("QuadCopter Model v0.2\n");

  float ypr2[DIM];
  int servonum=0;
  int servoval=0;
  int servoret=0;

  MPU_setup();
  //setup();

  servo=fopen("/dev/servoblaster","w");
  if (servo==NULL){
    printf("Opening /dev/servoblaster failed \n");
    exit(2);
  }


  for(;;){
    MPU_getYPR(ypr2);

    printf("yaw = %f, pitch = %f, roll = %f\n",
	   ypr2[YAW]*180/3.14, ypr2[PITCH]*180/3.14,
	   ypr2[ROLL]*180/3.14);


    // servoval=fabs(ypr2[ROLL]/1.6)*200;
    // servoret = SetServo(servonum,servoval);
    // if (servoret!=1){
    //   printf("Set servo failed \n");
    //   exit(2);
    // }

    usleep((DMP_FIFO_RATE)*5000);
  }

  fclose(servo);

  return 0;
}

