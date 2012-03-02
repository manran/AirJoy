/*
 * AirJoy.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-2-29
 *
 */

#ifndef __AIRJOY_H__
#define __AIRJOY_H__

#include <string>

#include "TcpRequestHandler.h"
#include "AirJoyConfig.h"

namespace airjoy
{

  class TcpMaster;
  class AirJoyProcessor;
  class AirJoyDelegate;

  class AirJoy : public TcpRequestHandler, public AirJoyConfig
  {
  public:
    AirJoy();
    virtual ~AirJoy();

    bool start(void);
    void stop(void);

    bool isRunning(void);
    int numberOfConnections(void);

    void setProcessor(AirJoyProcessor *processor);
    void setDelegate(AirJoyDelegate *delegate);

  public:
    
    // AirJoyConfig
    virtual int port(void);
    virtual void setPort(int port);

  private:

    // TcpRequestHandler
    virtual bool handleRequest(TcpWorker *pWorker, const char *data, int length);
  
  private:
    TcpMaster           *m_tcpMaster;
    AirJoyProcessor     *m_processor;
  };


}


#endif // __AIRJOY_H__