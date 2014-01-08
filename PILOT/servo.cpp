#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "servo.h"

Servo ESC;

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
  printf("Initialization of ESC...\n");
  if(fid_servo==NULL) return;

  //initialisation of ESC
  for (int i=0;i<4;i++){
    servo.servoval[i]=100;
  };

  servo.setServo();
  sleep(1);

  printf("                     ... DONE.\n");
}

void Servo::setServo()
{
  if(fid_servo==NULL) {
    printf("servoblaster not open !!!");
    exit(0);
  }

  for (int i=0;i<4;i++){
    fprintf(fid_servo, "%d=%d\n",m_servoId[i], servoval[i]);
    fflush(fid_servo);
  }
}
