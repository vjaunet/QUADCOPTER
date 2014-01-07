#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "pid.h"


PID yaw, pitch, roll;
PID yawRate, pitchRate, rollRate;


PID::PID()
{
  //PID constants
  m_Kd = 0;
  m_Ki = 0;
  m_Kp = 0;

  //PID variables
  m_err = 0;
  m_lasterr = 0;
  m_sum_err = 0;
  m_ddt_err = 0;
  m_lastInput= 0;
  m_outmax = 0;
  m_outmin = 0;

}

void PID::update_pid(float input)
{
   m_err = setpoint-input;

  //Integrating errors
  //winds up boundaries
  m_sum_err += m_err*m_Ki;
  if (m_sum_err > m_outmax) m_sum_err = m_outmax;
  if (m_sum_err < m_outmin) m_sum_err = m_outmin;


  //calculating error derivative
  //Input derivative is used to avoid derivative kick
  m_ddt_err = -m_Kd*(input - m_lastInput);

  //Calculation of the output
  //winds up boundaries
  output = m_Kp*m_err + m_sum_err + m_ddt_err;
  if (output > m_outmax) output = m_outmax;
  if (output < m_outmin) output = m_outmin;

  m_lasterr = m_err;
  m_lastInput= input;
}

void PID::set_Kpid(float Kp,float Ki, float Kd)
{
  m_Kd = Kp;
  m_Ki = Ki;
  m_Kp = Kd;
}

void PID::set_windup_bounds(float Min,float Max)
{
  m_outmax = Max;
  m_outmin = Min;
}
