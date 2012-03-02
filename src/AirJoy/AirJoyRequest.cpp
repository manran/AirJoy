/*
 * AirJoyRequest.cpp
 *
 *
 * jxfengzi@gmail.com
 *
 * 2012-3-1
 *
 */

#include "AirJoyRequest.h"
#include "AirJoyRequestThread.h"

#include <iostream> // for debug

using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

AirJoyRequest::AirJoyRequest()
{
  m_thread = NULL;
}

AirJoyRequest::~AirJoyRequest()
{
  this->stop();
}

bool AirJoyRequest::start(void)
{
  if (m_thread == NULL)
  {
    m_thread = new AirJoyRequestThread(this);
    m_thread->startThread();

    return true;
  }

  return false;
}

void AirJoyRequest::stop()
{
  if (m_thread != NULL)
  {
    m_thread->setThreadShouldExit();
    m_thread->notifyThreadWillExit();
    m_thread->waitForThreadToExit();

    delete m_thread;
    m_thread = NULL;
  }
}

AirJoySessionId AirJoyRequest::query(const std::string &ip, 
                                     int port, 
                                     const std::string &queryXmlns, 
                                     const std::string &appXmlns, 
                                     const std::string &param)
{
  AirJoyMessage *message = new AirJoyMessage;
  message->setType(AirJoyMessageTypeGet);
  message->setXmlns(queryXmlns);
  message->setAppXmlns(appXmlns);
  message->setAppParam(param);

  return m_thread->pushToQueue(ip, port, message);
}

void AirJoyRequest::sendAndWaitResponse(const std::string &ip, 
                                        int port, 
                                        AirJoyMessage *message, 
                                        AirJoySessionId sessionId)
{
  if (! this->InitSocket(ip, port))
    return;
  
  do
  {
    if (! this->connectServer(3))
      break;

    if (! this->sendToServer(message, sessionId, 3))
      break;

    if (! this->recvFromServer(3))
      break;
  }
  while (0);
 
  this->ReleaseSocket(); 
}

//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------

bool AirJoyRequest::InitSocket(const std::string &ip, 
                               int port)
{
  m_tcpSocketNo = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_tcpSocketNo < 0)
    return false;

  memset(&m_tcpServerAddress, 0, sizeof(m_tcpServerAddress));
  m_tcpServerAddress.sin_family      = AF_INET;
  m_tcpServerAddress.sin_addr.s_addr = inet_addr(ip.c_str());
  m_tcpServerAddress.sin_port        = htons(port);

  return true;
}

bool AirJoyRequest::ReleaseSocket(void)
{
#ifdef _WIN32
  ::closesocket(m_tcpSocketNo);
#else
  ::close(m_tcpSocketNo);
#endif

  return true;
}

bool AirJoyRequest::connectServer(int second)
{
  if (! this->setSocketNonblock())
    return false;

  int ret = ::connect(m_tcpSocketNo, (struct sockaddr *)&m_tcpServerAddress, sizeof(m_tcpServerAddress));

  // connect error
  if (ret < 0)
    return false;
  
  // connect completed immediately
  if (ret == 0)
    return true;

  // waiting for connect ok
  struct timeval  tv;
  int             maxfd;
  fd_set          readSet;
  fd_set          writeSet;

  FD_ZERO(&readSet);
  FD_SET(m_tcpSocketNo, &readSet);
  writeSet    = readSet;
  tv.tv_sec   = second;
  tv.tv_usec  = 0;
  maxfd = m_tcpSocketNo + 1;
  ret = ::select(maxfd, &readSet, &writeSet, NULL, second ? &tv : NULL);
  
  // timeout, connect error
  if (ret == 0)
    return false;

  if (FD_ISSET(m_tcpSocketNo, &readSet) || FD_ISSET(m_tcpSocketNo, &writeSet))
  {
  #if 0
    // UNP V1 P385, solaris peding error
    int error = 0;
    int len = sizeof(error);
    ret = ::getsocketopt(m_tcpSocketNo, SOL_SOCKET, SO_ERROR, &error, &len);
    if (ret < 0)
      return false;
  #endif

    return true;
  }

  // select error: m_tcpSocketNo not set
  return false;
}

bool AirJoyRequest::sendToServer(AirJoyMessage *message, 
                                 AirJoySessionId sessionId,
                                 int second)
{
  
  struct timeval  tv;
  int             maxfd;
  fd_set          writeSet;

  FD_ZERO(&writeSet);
  FD_SET(m_tcpSocketNo, &writeSet);
  tv.tv_sec   = second;
  tv.tv_usec  = 0;
  maxfd = m_tcpSocketNo + 1;
  int ret = ::select(maxfd, NULL, &writeSet, NULL, second ? &tv : NULL);
  
  // timeout
  if (ret == 0)
    return false;

  // select error
  if (ret < 0)
    return false;

  if (FD_ISSET(m_tcpSocketNo, &writeSet))
  {
    const char *buf = message->toText();
    int len = strlen(buf);

    int n = 0;
    n = ::send(m_tcpSocketNo, buf, len , 0);
    if (n < 0)
      return false;
    
    if (n == 0)
      return false;

    std::cout << "send: " << buf << std::endl;

    return true;
  }

  return false;
}

bool AirJoyRequest::recvFromServer(int second)
{
  struct timeval  tv;
  int             maxfd;
  fd_set          readSet;

  FD_ZERO(&readSet);
  FD_SET(m_tcpSocketNo, &readSet);
  tv.tv_sec   = second;
  tv.tv_usec  = 0;
  maxfd = m_tcpSocketNo + 1;
  int ret = ::select(maxfd, &readSet, NULL, NULL, second ? &tv : NULL);
  
  // timeout
  if (ret == 0)
    return false;

  // select error
  if (ret < 0)
    return false;

  if (FD_ISSET(m_tcpSocketNo, &readSet))
  {
    int n = 0;
    char buf[1024 * 20];
    int len = 1024 * 20;
    n = ::recv(m_tcpSocketNo, buf, len, 0);
    if (n < 0)
      return false;
    
    if (n == 0)
      return false;

    std::cout << "recv: " << buf << std::endl;

    return true;
  }

  return false;
}

bool AirJoyRequest::setSocketNonblock(void)
{
#ifdef _WIN32
  int ret =0;
  unsigned long ul = 1;
  ret = ::ioctlsocket(m_tcpSocketNo, FIONBIO, (unsigned long *)&ul);
  if (ret == SOCKET_ERROR)
    return false;
#else
  int val = 0; 
  val = ::fcntl(m_tcpSocketNo, F_GETFL, 0);
  ::fcntl(m_fd, F_SETFL, val | O_NONBLOCK);
#endif

  return true;
}