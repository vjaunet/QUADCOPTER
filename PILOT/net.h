#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class Socket{

 private:
  sockaddr_in m_address;
  unsigned short m_port;
  int m_socket;

 public:
  Socket();
  void create();
  void get_target(float& ,float& ,float& ,float&);


};

extern Socket wifi;

#endif
