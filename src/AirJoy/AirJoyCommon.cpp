/*
 * AirJoyCommon.cpp
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#include "AirJoyCommon.h"

#ifdef _WIN32
#   include <windows.h>
#else
#   include <time.h>
#   include <sys/time.h>
#   include <unistd.h>
#   include <signal.h>
#endif // _WIN32

using namespace airjoy;


void Util::airJoySleep(int second)
{
#ifdef _WIN32
    Sleep(second * 1000);
#else
    sleep(second);
#endif
}

bool Util::initSocket()
{
  static bool isAlreadyInit = false;

  if (isAlreadyInit)
    return true;

#ifdef WIN32
  WORD wVersionRequested;
  WSADATA wsaData;
  wVersionRequested = MAKEWORD( 2, 0 );

  if (WSAStartup(wVersionRequested, &wsaData) != 0)
    return false;
#else
  // Ignore SIGPIPE signal, so if browser cancels the request, it won't kill the whole process.
  (void) signal(SIGPIPE, SIG_IGN);
#endif

  isAlreadyInit = true;

  return true;
}
