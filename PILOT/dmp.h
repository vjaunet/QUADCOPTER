#ifndef dmp_
#define dmp_

class DMP {

 private:

  bool dmpReady;          // set true if DMP init was successful
  int YAW;
  int PITCH;
  int ROLL;
  int DIM;
  float lastval[3];
  float offset[3];

 public:
  DMP();
  void getAttitude();
  void set_com();
  void initialize();
  bool initialized;

  float ypr[3];
  float gyro[3];
};

extern DMP imu;

#endif
