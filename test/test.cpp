/*
 *
 * test.cpp
 *
 *
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include "AirJoy.h"

using namespace AirJoy;

int main()
{

#if 0
  HttpServer myHttpServer;

#if _WIN32
  std::string home = "f:/test";
#else
  std::string home = "/home/ouyang";
#endif

  myHttpServer.setPort(8080);
  myHttpServer.setDocumentRoot(home);
  myHttpServer.start();
#endif
  
#ifdef _WIN32
  while (1) 
    Sleep(1000);
#else
  while (1) 
    sleep(1000);
#endif

  return 0;
}
