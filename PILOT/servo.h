#ifndef SERVOS
#define SERVOS

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#define SERVO_MIN 1000;
#define YAW 0
#define PITCH 1
#define ROLL 2
#define DIM 3

class Servo {

private:
  int m_servoId[4];
  FILE* fid_servo;

public:
  Servo();
  void open_blaster();
  void init();
  void update(float, float []);
  void setServo();

  int servoval[4];
};

extern Servo ESC;

#endif
