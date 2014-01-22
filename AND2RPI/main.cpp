#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "net.h"

using namespace net;

int main( int argc, char * argv[] )
{

  int target[4];
  int port=7000;
  Socket socket;


  //open socket
  if ( !socket.Open( port ) )
    printf( "failed to create socket!\n" );

  //Wait for packet to arrive
  socket.get_target(target);

  //output
  printf("thr: %d yaw: %d pitch: %d roll:%d\n",
	  target[0],target[1],target[2],target[3]);

  return 0;
}
