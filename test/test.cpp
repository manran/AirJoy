/*
 *
 * test.cpp
 *
 *
 */

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

#include "AirJoy/AirJoy.h"

using namespace airjoy;

int main()
{
  AirJoy myAirJoy;

  myAirJoy.setPort(9999);
  myAirJoy.start();
  
  std::cout << "AirJoy, port: " << myAirJoy.port() << std::endl;

#ifdef _WIN32
  while (1) 
    Sleep(1000);
#else
  while (1) 
    sleep(1000);
#endif

  return 0;
}
