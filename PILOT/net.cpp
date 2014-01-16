/*

  Socekt class
  author : vincent jaunet
  date : 10-01-2013

  Description :
  The Socket class contains all the necessary routine to set
  up and use a UDP network serve on the drone waiting for
  the remote client to send request.
  The output should be parsed in order to retrieve
  the desired attitude from the remote (see parse.*)

*/


#include "net.h"

Socket remote;

Socket::Socket()
{
  data[0] = '\0';
  m_lastdata[0] = '\0';
  m_last_type = -1;
  m_port = 7000;
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons( (unsigned short) m_port );
}

void Socket::set_port(int port){
  //set the port to desired value
  m_port = port;
}


void Socket::create()
{

  //Opening socket
  m_socket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
  if ( m_socket <= 0 )
    {
      printf( "Failed to create socket\n" );
      exit(EXIT_FAILURE);
    }

  //Binding to desired port number
  if ( bind( m_socket, (const sockaddr*) &m_address, sizeof(sockaddr_in) ) < 0 )
    {
      printf( "failed to bind socket\n" );
      exit(EXIT_FAILURE);
    }

  //setting Socket to non blocking mode
  int nonBlocking = 1;
  if ( fcntl( m_socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
    {
      printf( "failed to set non-blocking socket\n" );
      exit(EXIT_FAILURE);
    }

  printf( "Succeed to create socket\n\n" );
}


int Socket::get_cmd(){

  int type=0;

  //returns 1 for Start(Initialize)
  //retunrs 10 for yaw PID constants
  //retunrs 11 for yawrate PID constants
  //retunrs 12 for PR PID constants
  //retunrs 13 for PRrate PID constants

  //returns 0 for rcinputs (default)

  int size = sizeof(data);
  assert( size > 0 );

  if ( m_socket == 0 )
    printf("Socket is closed...");

  int received_bytes = -1;
  sockaddr_in from;
  socklen_t fromLength = sizeof( from );

  received_bytes = recvfrom( m_socket, data, size, 0,
				 (sockaddr*)&from,
				 &fromLength);

  if (received_bytes <= 0){
    for (int i=0;i<256;i++){
      data[i] = m_lastdata[i];
      type = m_last_type;
    }}

  std::string packet( reinterpret_cast< char const* > (data));
  std::istringstream ss(packet);

  std::string sub;
  ss >> sub;

  do{
    if (sub == "START"){
      type = 1;
      break;
    }else if (sub == "EXIT"){
      exit(0);
      break;
    } else if(sub == "pid"){
       ss >> sub;
       if (sub == "yaw_rate"){
	 type = 10;
	 break;
       }else if (sub == "yaw_stab"){
	 type = 11;
	 break;
       }else if (sub == "pr_stab"){
	 type = 12;
	 break;
       }else if (sub == "pr_rate"){
	 type = 13;
	 break;
       }
    } else { break; }
  }while(ss);

  m_last_type = type;
  return(type);

}



