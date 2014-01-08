#include "net.h"

Socket wifi;

Socket::Socket()
{
  m_port = 7000;
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons( (unsigned short) m_port );
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

void Socket::get_target(float &t,float &y,float &p,float &r)
{
  unsigned char data[256];
  int size = sizeof(data);
  assert( size > 0 );

  if ( m_socket == 0 )
    printf("Socket is closed...");

  int received_bytes = -1;
  //do{
      sockaddr_in from;
      socklen_t fromLength = sizeof( from );

      received_bytes = recvfrom( m_socket, data, size, 0,
				 (sockaddr*)&from, &fromLength );
      //}while ( received_bytes <= 0 );

      if (received_bytes < 0) {
	t=last_t;
	y=last_y;
	p=last_p;
	r=last_r;
	return;
      };

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
	last_t = t;
      }
      else if(sub == "\"yaw\":"){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	y = (float) cmd;
	last_y = y;
      }
      else if(sub == "\"pitch\":"){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	p = (float) cmd;
	last_p = p;
      }
      else if(sub == "\"roll\":"){
	ss >> sub;
	std::istringstream( sub ) >> cmd;
	r = (float) cmd;
	last_r = r;
      }
    } while (ss);
}



