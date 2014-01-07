#ifndef pid
#define pid

class PID {

private:
  //PID constants
  float m_Kp;
  float m_Ki;
  float m_Kd;

  //PID constants
  float m_err;
  float m_lasterr;
  float m_sum_err;
  float m_ddt_err;
  float m_lastInput;
  float m_outmax;
  float m_outmin;

public:
  PID();
  void update_pid(float);
  void set_Kpid(float, float, float);
  void set_windup_bounds(float, float);

  float setpoint;
  float output;

};


extern PID yaw, pitch, roll;
extern PID yawRate, pitchRate, rollRate;

#endif
