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
#include "AirJoy/AirJoyDelegate.h"
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
    AirJoySessionId sessionId = 0;
    sessionId = myAirJoyRequest.query("192.168.0.201", 
                                      9999, 
                                      "http://www.airjoy.cn/query",
                                      "http://www.airjoy.cn/getsharedfolder",
                                      "/video");
  
    std::cout << "query sessionId: " << sessionId << std::endl;

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

    virtual void didNotInitSocket(AirJoySessionId sessionId) 
    {
      std::cout << "didNotInitSocket: " << sessionId << std::endl;
    }

    virtual void didNotConnectServer(AirJoySessionId sessionId) 
    {
      std::cout << "didNotConnectServer: " << sessionId << std::endl;
    }

    virtual void didNotSendToServer(AirJoySessionId sessionId) 
    {
      std::cout << "didNotSendToServer: " << sessionId << std::endl;
    }

    virtual void didNotRecvFromServer(AirJoySessionId sessionId) 
    {
      std::cout << "didNotRecvFromServer: " << sessionId << std::endl;
    }

    virtual void didConnectServer(AirJoySessionId sessionId) 
    {
      std::cout << "didConnectServer: " << sessionId << std::endl;
    }

    virtual void didSendToServer(AirJoySessionId sessionId) 
    {
      std::cout << "didSendToServer: " << sessionId << std::endl;
    }

    virtual void didRecvFromServer(AirJoyMessage &response, AirJoySessionId sessionId) 
    {
      std::cout << "didRecvFromServer: " << sessionId << std::endl;
      std::cout << response.toText() <<  std::endl;
    }
};

class AirDelegate : public AirJoyDelegate
{
public:
  AirDelegate() {}
  virtual ~AirDelegate() {}

  virtual void willStart(void) 
  {
    std::cout << "AirJoy willStart" << std::endl;
  }

  virtual void didStart(void)
  {
    std::cout << "AirJoy didStart" << std::endl;
  }

  virtual void didNotStart(void)
  {
    std::cout << "AirJoy didNotStart" << std::endl;
  }

  virtual void willStop(void)
  {
    std::cout << "AirJoy willStop" << std::endl;
  }

  virtual void didStop(void)
  {
    std::cout << "AirJoy didStop" << std::endl;
  }
};


int main()
{
  // processor
  AirJoyProcessor *myProcessor = new AirJoyProcessor;
  ProcessorDelegate *myProcessorDelegate = new ProcessorDelegate;
  myProcessor->setDelegate(myProcessorDelegate);

  // Start AirJoy Server
  AirDelegate *myAirDelegate = new AirDelegate;
  myAirJoy.setProcessor(myProcessor);
  myAirJoy.setDelegate(myAirDelegate);

  myAirJoy.setPort(9999);
  myAirJoy.start();

  std::cout << "AirJoy, port: " << myAirJoy.port() << std::endl;

  // Start AirJoy Request
  RequestDelegate *myRequestDelegate = new RequestDelegate;
  myAirJoyRequest.setDelegate(myRequestDelegate);
  myAirJoyRequest.start();

  command();

  delete myRequestDelegate;
  delete myAirDelegate;
  delete myProcessorDelegate;
  delete myProcessor;

  return 0;
}
