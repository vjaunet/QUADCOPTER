/*
  ------------------
   Quadcopter Pilot
  ------------------
  author : vincent jaunet
  mail : vincent.jaunet(AT)hotmail(DOT)fr

  - Initialization of PID
  - Initialization of ESC
  - Initialization of MPU6050
  - Initialization of Remote socket


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

  //set PID constants from command line options
  float kp_,ki_,kd_;
  float KPID[6];
  if (cli.CLparser(argc, argv)){
    cli.getKPID(KPID);
    for (int i=0;i<DIM;i++){
      yprSTAB[i].set_Kpid(KPID[0],KPID[1],KPID[2]);
      yprRATE[i].set_Kpid(KPID[3],KPID[4],KPID[5]);
    }
  }

  //initializing Network communication
  remote.create();

  /* Waiting fo Start command */
  while (true){

    switch(remote.get_cmd()){
      //returns 1 for Start(Initialize)
      //returns 2 for Initialize
      //returns 10 for yawstab PID constants
      //returns 11 for yawrate PID constants
      //returns 12 for PRstab PID constants
      //returns 13 for PRrate PID constants

    case 10:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      yprSTAB[0].set_Kpid(kp_,ki_,kd_);
      break;
    case 11:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      yprRATE[0].set_Kpid(kp_,ki_,kd_);
      break;
    case 12:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      yprSTAB[1].set_Kpid(kp_,ki_,kd_);
      yprSTAB[2].set_Kpid(kp_,ki_,kd_);
      break;
    case 13:
      //set pid constants
      parser.parse(remote.data,kp_,ki_,kd_);
      yprRATE[1].set_Kpid(kp_,ki_,kd_);
      yprRATE[2].set_Kpid(kp_,ki_,kd_);
      break;

    case 2:
      //intialization of IMU
      imu.set_com();
      imu.initialize();
      break;

    case 1:
      //Remote says "Start"
      if (!imu.initialized){
	printf("DMP not Initalized\n Can't start...\n");
	break;
      }

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
