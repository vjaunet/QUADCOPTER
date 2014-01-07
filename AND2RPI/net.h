#ifndef NET
#define NET

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>

namespace net{

  //void wait( float seconds ) { usleep( (int) ( seconds * 1000000.0f ) );}


  // internet address

  class Address
  {
  public:

    Address()
      {
	address = 0;
	port = 0;
      }

    Address( unsigned char a, unsigned char b, unsigned char c, unsigned char d,
	     unsigned short port )
      {
	this->address = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
	this->port = port;
      }

    Address( unsigned int address, unsigned short port )
      {
	this->address = address;
	this->port = port;
      }

    unsigned int GetAddress() const
    {
      return address;
    }

    unsigned char GetA() const
    {
      return ( unsigned char ) ( address >> 24 );
    }

    unsigned char GetB() const
    {
      return ( unsigned char ) ( address >> 16 );
    }

    unsigned char GetC() const
    {
      return ( unsigned char ) ( address >> 8 );
    }

    unsigned char GetD() const
    {
      return ( unsigned char ) ( address );
    }

    unsigned short GetPort() const
    {
      return port;
    }

    bool operator == ( const Address & other ) const
    {
      return address == other.address && port == other.port;
    }

    bool operator != ( const Address & other ) const
    {
      return ! ( *this == other );
    }

  private:

    unsigned int address;
    unsigned short port;
  };

  // sockets

  class Socket
  {
  public:

    Socket()
      {
	socket = 0;
      }

    ~Socket()
      {
	Close();
      }

    bool Open( unsigned short port )
    {
      assert( !IsOpen() );

      // create socket

      socket = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

      if ( socket <= 0 )
	{
	  printf( "failed to create socket\n" );
	  socket = 0;
	  return false;
	}

      // bind to port

      sockaddr_in address;
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = INADDR_ANY;
      address.sin_port = htons( (unsigned short) port );

      if ( bind( socket, (const sockaddr*)
		 &address, sizeof(sockaddr_in) ) < 0 )
	{
	  printf( "failed to bind socket\n" );
	  Close();
	  return false;
	}

      // set non-blocking io

      int nonBlocking = 1;
      if ( fcntl( socket, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
	{
	  printf( "failed to set non-blocking socket\n" );
	  Close();
	  return false;
	}

      return true;
    }

    void Close()
    {
      if ( socket != 0 )
	{
	  close( socket );
	  socket = 0;
	}
    }

    bool IsOpen() const
    {
      return socket != 0;
    }

    bool Send( const Address & destination, const void * data, int size )
    {
      assert( data );
      assert( size > 0 );

      if ( socket == 0 )
	return false;

      sockaddr_in address;
      address.sin_family = AF_INET;
      address.sin_addr.s_addr = htonl( destination.GetAddress() );
      address.sin_port = htons( (unsigned short) destination.GetPort() );

      int sent_bytes = sendto( socket, (const char*)data, size, 0,
			       (sockaddr*)&address, sizeof(sockaddr_in) );

      return sent_bytes == size;
    }

    void extract_target(unsigned char buffer[],int target[])
    {
      //Processing packet
      std::string packet( reinterpret_cast< char const* > (buffer));
      std::istringstream ss(packet);
      //printf("%s \n", packet.c_str());

      //Getting target values from packet
      do
	{
	  std::string sub;
	  ss >> sub;

	  if (sub == "\"thr\":" ){
	    ss >> sub;
	    std::istringstream( sub ) >> target[0];
	  }
	  else if(sub == "\"yaw\":"){
	    ss >> sub;
	    std::istringstream( sub ) >> target[1];
	  }
	  else if(sub == "\"pitch\":"){
	    ss >> sub;
	    std::istringstream( sub ) >> target[2];
	  }
	  else if(sub == "\"roll\":"){
	    ss >> sub;
	    std::istringstream( sub ) >> target[3];
	  }
	} while (ss);
    }


    void get_target( int target[])
    {
      unsigned char data[256];
      int size = sizeof(data);
      assert( data );
      assert( size > 0 );

      if ( socket == 0 )
	printf("Socket is closed...");

      int received_bytes = -1;
      do
	{
	  sockaddr_in from;
	  socklen_t fromLength = sizeof( from );

	  received_bytes = recvfrom( socket, data, size, 0,
				     (sockaddr*)&from, &fromLength );
	}while ( received_bytes <= 0 );


      //get target from packet data
      extract_target(data, target);


    }

  private:

    int socket;
  };
}
#endif
