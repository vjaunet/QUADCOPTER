#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "servo.h"

Servo::Servo()
{
  m_servoId[0] = 0;
  m_servoId[1] = 1;
  m_servoId[2] = 2;
  m_servoId[3] = 3;

}

void Servo::open_blaster()
{
  fid_servo=fopen("/dev/servoblaster","w");
  if (fid_servo==NULL){
    printf("Opening /dev/servoblaster failed \n");
    exit(2);
  }
}

void Servo::init(Servo servo)
{
  if(fid_servo==NULL) return;

  //initialisation of ESC
  int servoval[4];
  int i;
  for (i=0;i<4;i++){
    servoval[i]=1000;
  };

  servo.set(servoval);

  sleep(2);
}

void Servo::set(int* ServoPosition)
{
  int i;
  if(fid_servo==NULL) return;
  for (i=0;i<4;i++){
    fprintf(fid_servo, "%d=%d\n",m_servoId[i],ServoPosition[i]);
    fflush(fid_servo);
  }
}
