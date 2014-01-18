/*

   PID Controller

   author : vincent JAUNET
   mail : vincent.jaunet@hotmail.fr
   date : 10-01-2013

   Description:
   the PID class is a collection of routine necessary
   to perform a PID control based on setpoints (remote control),
   inputs (measured attitude).
   Can be used for any type of system and features :
   - Derivative on measurement
   - Windsup of integral errors

*/

#include "pid.h"


PID yprSTAB[3];
PID yprRATE[3];


PID::PID()
{
  //PID constants
  m_Kd = 0;
  m_Ki = 0;
  m_Kp = 0.01;

  //PID variables
  m_err = 0;
  m_lasterr = 0;
  m_sum_err = 0;
  m_ddt_err = 0;
  m_lastInput= 0;
  m_outmax = 20;
  m_outmin = -20;
}

int PID::update_pid(float setpoint, float input, float dt)
{

  //Computes error
  m_err = setpoint-input;

  //Integrating errors
  m_sum_err += m_err * m_Ki * dt;

  //calculating error derivative
  //Input derivative is used to avoid derivative kick
  m_ddt_err = -m_Kd / dt * (input - m_lastInput);

  //Calculation of the output
  //winds up boundaries
  m_output = m_Kp*m_err + m_sum_err + m_ddt_err;
  if (m_output > m_outmax) {
    //winds up boundaries
    m_sum_err -= m_output - m_outmax;
    m_output = m_outmax;
  }else if (m_output < m_outmin) {
    //winds up boundaries
    m_sum_err += m_outmin - m_output;
    m_output = m_outmin;
  }

  m_lasterr = m_err;
  m_lastInput= input;

  //printf("setpt %f input %f outpt %f\n", m_Kp, m_Ki, m_Kd);
  //printf("setpt %f input %f outpt %f\n", setpoint, input, output);

  return (int)m_output;
}

void PID::set_Kpid(float Kp,float Ki, float Kd)
{
  m_Kp = Kp;
  m_Ki = Ki;
  m_Kd = Kd;
}

void PID::set_windup_bounds(float Min,float Max)
{
  m_outmax = Max;
  m_outmin = Min;
}
