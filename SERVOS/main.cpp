#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

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



int main(int argc, char *argv[])
{
  printf("Hello world!\n");
  printf("QuadCopter Model v0.2\n");

  int servoval[4];
  Servo ESC;

  ESC.open_blaster();
  ESC.init(ESC);

  sleep(0.5);
  for(;;){

   servoval[0]=1120;
   servoval[1]=1120;
   servoval[2]=1120;
   servoval[3]=1120;
   ESC.set(servoval);

   sleep(0.5);
  }

  return 0;
}

