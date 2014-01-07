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
  printf("QuadCopter Model v0.3\n");
  printf("----------------------\n");
  printf("\n");

  //command line options
  cli.CLparser(argc, argv);

  //initializing Network communication
  wifi.create();

  //intialization of IMU
  imu.set_com();
  imu.initialize();

  //Initializing ESCs
  ESC.open_blaster();
  ESC.init(ESC);

  //things are getting started !
  Timer.start();
  for(;;){

    sleep(1000);

  }//PID LOOP

  return 0;
}
