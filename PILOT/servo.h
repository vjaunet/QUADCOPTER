#ifndef SERVOS
#define SERVOS

class Servo {

private:
  int m_servoId[4];
  FILE* fid_servo;

public:
  Servo();
  void open_blaster();
  void init(Servo servo);
  void setServo();

  int servoval[4];
};

extern Servo ESC;

#endif
