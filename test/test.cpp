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
#include "AirJoy/AirJoyProcessor.h"
#include "AirJoy/AirJoyProcessorDelegate.h"
#include "AirJoy/AirJoyRequestDelegate.h"

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

class ProcessorDelegate : public AirJoyProcessorDelegate
{
public:
  ProcessorDelegate() {}
  virtual ~ProcessorDelegate() {}

  // return true if you handle this request, otherwise return false;
  virtual bool didReceiveRequest(AirJoyProcessor *processor, AirJoyMessage &request) 
  { 
    std::string result("hello airjoy");

    processor->responseForRequest(request, AirJoyMessageTypeResult, result);

    return true;
  }
};

class RequestDelegate : public AirJoyRequestDelegate
{
public:
     RequestDelegate() {}
    virtual ~RequestDelegate() {}

    virtual void didNotInitSocket(void) 
    {
      std::cout << "didNotInitSocket" << std::endl;
    }

    virtual void didNotConnectServer(void) 
    {
      std::cout << "didNotConnectServer" << std::endl;
    }

    virtual void didNotSendToServer(void) 
    {
      std::cout << "didNotSendToServer" << std::endl;
    }

    virtual void didNotRecvFromServer(void) 
    {
      std::cout << "didNotRecvFromServer" << std::endl;
    }

    virtual void didConnectServer(void) 
    {
      std::cout << "didConnectServer" << std::endl;
    }

    virtual void didSendToServer(void) 
    {
      std::cout << "didSendToServer" << std::endl;
    }

    virtual void didRecvFromServer(AirJoyMessage &response, AirJoySessionId sessionId) 
    {
      std::cout << "didRecvFromServer: " << sessionId << std::endl;
      std::cout << response.toText() <<  std::endl;
    }
};


int main()
{
  // processor
  AirJoyProcessor *processor = new AirJoyProcessor;
  processor->setDelegate(new ProcessorDelegate);

  // Start AirJoy Server
  myAirJoy.setProcessor(processor);

  myAirJoy.setPort(9999);
  myAirJoy.start();
  
  std::cout << "AirJoy, port: " << myAirJoy.port() << std::endl;

  // Start AirJoy Request
  myAirJoyRequest.start();
  myAirJoyRequest.setDelegate(new RequestDelegate);

  command();

  return 0;
}
