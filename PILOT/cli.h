#ifndef __CLI
#define __CLI

#include <string.h>
#include <string>

class CLI {

 private:
  float m_AngKp;
  float m_AngKi;
  float m_AngKd;

  float m_RateKp;
  float m_RateKi;
  float m_RateKd;


 public:
  CLI();

  void CLparser(int, char *[]);
  void getAngleKs(float,float,float);
};

extern CLI cli;

#endif
