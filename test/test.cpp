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
#include "AirJoy/AirJoyRequest.h"

using namespace airjoy;

bool            gLoop = true;
AirJoy          myAirJoy;
AirJoyRequest   myAirJoyRequest;

void  parseCommand(const std::string &cmd)
{
  if (cmd == "help" || cmd == "h")
  {
    std::cout << "---------- help ----------" << std::endl;
    std::cout << "help(h) -- help infomation" << std::endl;
    std::cout << "exit(x) -- exit" << std::endl;
    std::cout << "req(r)  -- send request" << std::endl;
    return;
  }

  if (cmd == "exit" || cmd == "x")
  {
    myAirJoyRequest.stop();
    myAirJoy.stop();
    gLoop = false;
    return;
  }

  if (cmd == "req" || cmd == "r")
  {
    myAirJoyRequest.query("192.168.0.201", 
                          9999, 
                          "http://www.airjoy.cn/query",
                          "http://www.airjoy.cn/getsharedfolder",
                          "/video");
    return;
  }
}

void command()
{
  std::string buf;

  while (gLoop)
  {
    std::cout << "> ";
    buf.clear();
    std::cin >> buf;
    parseCommand(buf);
  }
}

int main()
{
  myAirJoy.setPort(9999);
  //myAirJoy.start();
  
  std::cout << "AirJoy, port: " << myAirJoy.port() << std::endl;

  myAirJoyRequest.start();
  
  command();

  return 0;
}
