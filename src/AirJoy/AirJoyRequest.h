/*
 * AirJoyRequest.h
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-3-1
 *
 */

#ifndef __AIRJOY_REQUEST_H__
#define __AIRJOY_REQUEST_H__

#include "AirJoyCommon.h"

#include <string>

#ifdef _WIN32
#   include <winsock.h>
#else
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <pthread.h>
#endif

namespace airjoy
{

  class AirJoyMessage;
  class AirJoyRequestThread;

  class AirJoyRequest
  {
  public:
    AirJoyRequest();
    virtual ~AirJoyRequest();
  
    bool start();
    void stop();

    AirJoySessionId query(const std::string &ip, 
                          int port, 
                          const std::string &queryXmlns, 
                          const std::string &appXmlns, 
                          const std::string &param);

    void sendAndWaitResponse(const std::string &ip, 
                             int port, 
                             AirJoyMessage *message, 
                             AirJoySessionId sessionId);

  private:
    bool InitSocket(const std::string &ip, int port);
    bool ReleaseSocket(void);
    bool connectServer(int second);
    bool sendToServer(AirJoyMessage *message, AirJoySessionId sessionId, int second);
    bool recvFromServer(int second);
    bool setSocketNonblock(void);
    
  private:
    AirJoyRequestThread       *m_thread;

    int                        m_tcpSocketNo;
    struct sockaddr_in         m_tcpServerAddress;
  };

}

#endif // __AIRJOY_REQUEST_H__