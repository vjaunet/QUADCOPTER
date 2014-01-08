#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#include "global.h"

#include "net.h"
#include "servo.h"
#include "pid.h"
#include "dmp.h"
#include "timer.h"
#include "cli.h"


//-------------------------------------
//--------- Main-----------------------

int main(int argc, char *argv[])
{
  printf("Hello world!\n");
  printf("QuadCopter Pilot v0.1\n");
  printf("----------------------\n");
  printf("\n");

  //command line options
  cli.CLparser(argc, argv);

  //set PID constants
  float KPID[6];
  cli.getKPID(KPID);

  for (int i=0;i<DIM;i++){
    ypr[i].set_Kpid(KPID[0],KPID[1],KPID[2]);
  }


  //initializing Network communication
  wifi.create();

  //intialization of IMU
  imu.set_com();
  //imu.initialize();

  //Initializing ESCs
  ESC.open_blaster();
  ESC.init(ESC);

  //things are getting started !
  Timer.start();
  for(;;){
    sleep(1000);
  }

  return 0;
}
