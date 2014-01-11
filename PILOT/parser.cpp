/*

  Parser for data from QuadControl app
  ------------------------------------

  take the char data as an input and
  ouputs floating points values

  author : vincennt JAUNET
  License MIT


 */

#include "parser.h"

Parser parser;

//constructor
Parser::Parser()
{
}

//destructor
Parser::~Parser()
{
}

void Parser::parse(unsigned char data[256],float &t,float ypr[]){
  //returns thrust, yaw, pitch and roll walues form char data

  //Processing packet
  std::string packet( reinterpret_cast< char const* > (data));
  std::istringstream ss(packet);
  //printf("%s \n", packet.c_str());

  //Getting target values from packet
  do
    {
      std::string sub;
      ss >> sub;
      int cmd;

      if (sub == "\"thr\":" ){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	t = (float) (cmd + 110);
      }
      else if(sub == "\"yaw\":"){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	ypr[0] = (float) cmd;
      }
      else if(sub == "\"pitch\":"){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	ypr[1] = (float) cmd;
      }
      else if(sub == "\"roll\":"){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	ypr[2] = (float) cmd;
      }
    } while (ss);
}


void Parser::parse(unsigned char data[],float &ki,float &kp,float &kd){
  //returns thrust, yaw, pitch and roll walues form char data

  //Processing packet
  std::string packet( reinterpret_cast< char const* > (data));
  std::istringstream ss(packet);
  //  printf("%s \n", packet.c_str());

  //Getting target values from packet
  do
    {
      std::string sub;
      ss >> sub;

      if (sub == "ki" ){
	ss >> sub;
	ki = ::atof(sub.c_str());
      }
      else if(sub == "kp"){
	ss >> sub;
	kp = ::atof(sub.c_str());
      }
      else if(sub == "kd"){
	ss >> sub;
	kd = ::atof(sub.c_str());
      }
    } while (ss);
}


