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
  class AirJoyRequestDelegate;

  class AirJoyRequest
  {
  public:
    AirJoyRequest();
    virtual ~AirJoyRequest();

    void setDelegate(AirJoyRequestDelegate *requestDelegate);

    bool start();
    void stop();

    AirJoySessionId query(const std::string &ip, 
                          int port, 
                          const std::string &queryXmlns, 
                          const std::string &appXmlns, 
                          const std::string &param);

    void sendRequestAndWaitResponse(const std::string &ip, 
                                    int port, 
                                    AirJoyMessage *request, 
                                    AirJoySessionId sessionId);

  private:
    bool initSocket(const std::string &ip, int port);
    bool releaseSocket(void);
    bool connectServer(int second);
    bool sendToServer(AirJoyMessage *message, AirJoySessionId sessionId, int second);
    bool recvFromServer(char *buf, int len, int second);
    bool setSocketNonblock(void);
    
  private:
    AirJoyRequestThread       *m_thread;
    AirJoyRequestDelegate     *m_delegate;

    int                        m_tcpSocketNo;
    struct sockaddr_in         m_tcpServerAddress;
    int                        m_timeout;
  };

}

#endif // __AIRJOY_REQUEST_H__