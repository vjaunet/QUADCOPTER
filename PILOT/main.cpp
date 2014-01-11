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
  float kp_,ki_,kd_;
  float KPID[6];
  cli.getKPID(KPID);

  for (int i=0;i<DIM;i++){
    ypr[i].set_Kpid(KPID[0],KPID[1],KPID[2]);
  }

  //initializing Network communication
  remote.create();

  /* Waiting fo Start command */
  while (true){

    switch(remote.get_cmd()){
    case 10:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      ypr[0].set_Kpid(kp_,ki_,kd_);
      break;
    case 11:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      yprRate[0].set_Kpid(kp_,ki_,kd_);
      break;
    case 12:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      ypr[1].set_Kpid(kp_,ki_,kd_);
      ypr[2].set_Kpid(kp_,ki_,kd_);
      break;
    case 13:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      yprRate[1].set_Kpid(kp_,ki_,kd_);
      yprRate[2].set_Kpid(kp_,ki_,kd_);
      break;


    case 1:
      //Remote says "Start"

	//intialization of IMU
	imu.set_com();
	//imu.initialize();

	//Initializing ESCs
	ESC.open_blaster();
	ESC.init(ESC);

	//things are getting started !
	Timer.start();
	for (;;){
	  sleep(1000);
	}

      }//end switch
  }//end

  return 0;
}
