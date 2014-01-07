#include <string.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "cli.h"

CLI cli;

CLI::CLI(){

  m_AngKp=0;
  m_AngKi=0;
  m_AngKd=0;

  m_RateKp=0;
  m_RateKi=0;
  m_RateKd=0;

}


void CLI::CLparser(int argc, char *argv[]){
  if (argc <=1)
    {
      return;
    }

  for (int n=1;n<argc;n+=2){
    if (strcmp(argv[n],"--AngKp")==0)
      {
	m_AngKp = atof(argv[n+1]);
	break;

      } else if (strcmp(argv[n],"--AngKi")==0)
      {
	m_AngKi = atof(argv[n+1]);
	break;

      } else if (strcmp(argv[n],"--AngKd")==0)
      {
	m_AngKd = atof(argv[n+1]);
	break;

      } else if (strcmp(argv[n],"--RateKp")==0)
      {
	m_RateKp = atof(argv[n+1]);
	break;

      } else if (strcmp(argv[n],"--RateKi")==0)
      {
	m_RateKi = atof(argv[n+1]);
	break;

      } else if (strcmp(argv[n],"--RateKd")==0)
      {
	m_RateKd = atof(argv[n+1]);
	break;

      } else {
      printf("Invalid options");
      exit(1);
    }

    }//for n=1 to argc
}

void CLI::getAngleKs(float, float, float){

}
