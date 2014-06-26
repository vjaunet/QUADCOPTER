#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <termios.h>
#include <fcntl.h>
#include "kbhit.h"

#include "global.h"
#include "servo.h"

//-------------------------------------
//--------- Main-----------------------


//-------------------------------------
//TODO : CREATE a SERVO.h file in order to make subroutines
//
//
// If you set a servo width to 0 it turns off the servo output, without changing
// the current servo position.
// The code supports 8 servos, the control signals of which should be connected
// to P1 header pins as follows:
//      Servo number    GPIO number   Pin in P1 header
//           0               4             P1-7
//           1              17             P1-11
//           2              18             P1-12
//           3             21/27           P1-13
//           4              22             P1-15
//           5              23             P1-16
//           6              24             P1-18
//           7              25             P1-22
// P1-13 is connected to either GPIO-21 or GPIO-27, depending on board revision.
//
//
//-------------------------------------

using namespace std;

int main(int argc, char *argv[])
{
  printf("Hello world!\n");
  printf("QuadCopter Model v0.2\n");

  int servoval[4];
  Servo ESC;
  bool hit=true;
  keyboard kb1,kb2,kb3,kb4;

  ESC.open_blaster();
  //ESC.init(ESC);

  // sleep(0.5);
  // printf("\n");
  // printf("Calibrating SimonK ESC...\n");
  // printf("   Sending 2ms pulse, Hint keyboard when ESC beeped\n");

  // while(hit){

  //  servoval[0]=2000;
  //  servoval[1]=2000;
  //  servoval[2]=2000;
  //  servoval[3]=2000;
  //  ESC.set(servoval);

  //  if (kb1.kbhit()!=0) {
  //    hit=false;
  //  }

  //  sleep(0.5);
  // }

  // printf("\n");
  // printf("   Sending 1ms pulse, Hint keyboard when ESC beeped\n");
  // hit=true;

  // while(hit){

  //  servoval[0]=1000;
  //  servoval[1]=1000;
  //  servoval[2]=1000;
  //  servoval[3]=1000;
  //  ESC.set(servoval);

  //  if (kb2.kbhit()!=0) {
  //    hit=false;
  //  }

  //  sleep(0.5);
  // }

  // printf("\n");
  // printf("   Ready for test: 1.2ms, Pess any key\n");
  // hit=true;

  // while (hit){
  //   if (kb3.kbhit()!=0) {
  //     hit=false;
  //   }
  // }

  printf("   Arming ESC, Pess any key when beeped\n");

  hit=true;
  while(hit){

    servoval[0]=1000;
    servoval[1]=1000;
    servoval[2]=1000;
    servoval[3]=1000;
    ESC.set(servoval);

    if (kb1.kbhit()!=0) {
      hit=false;
    }

    sleep(0.5);
  }

  printf("   Sending 1.2ms, Pess any key to stop\n");

  hit=true;
  while(hit){

    int cmd=1100;

    servoval[0]=cmd;
    servoval[1]=cmd;
    servoval[2]=cmd;
    servoval[3]=cmd;
    ESC.set(servoval);

    if (kb4.kbhit()!=0) {
      hit=false;
    }

    sleep(0.5);
  }


    servoval[0]=1000;
    servoval[1]=1000;
    servoval[2]=1000;
    servoval[3]=1000;
    ESC.set(servoval);

  return 0;
}



