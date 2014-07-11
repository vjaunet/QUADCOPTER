#include "kbhit.h"
#include <unistd.h> // read()

keyboard::keyboard()
{
  tcgetattr(0,&initial_settings);
  new_settings = initial_settings;
  new_settings.c_lflag &= ~ICANON;
  new_settings.c_lflag &= ~ECHO;
  new_settings.c_lflag &= ~ISIG;
  new_settings.c_cc[VMIN] = 1;
  new_settings.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &new_settings);
  peek_character=-1;
}

keyboard::~keyboard()
{
  tcsetattr(0, TCSANOW, &initial_settings);
}

int keyboard::kbhit()
{
  unsigned char ch;
  int nread;

  if (peek_character != -1) return 1;
  new_settings.c_cc[VMIN]=0;
  tcsetattr(0, TCSANOW, &new_settings);
  nread = read(0,&ch,1);
  new_settings.c_cc[VMIN]=1;
  tcsetattr(0, TCSANOW, &new_settings);

  if (nread == 1)
    {
      peek_character = ch;
      return 1;
    }
  return 0;
}
