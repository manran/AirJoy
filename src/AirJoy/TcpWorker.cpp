/*
 * TcpWorker.cpp
 *
 * A tiny tcp server framework
 *
 * jxfengzi@gmail.com
 *
 * 2011-8-9
 *
 */


#include "TcpWorker.h"
#include "TcpRequestHandler.h"

#include <iostream> // for debug

#ifdef _WIN32
  typedef int socklen_t;
#else
#       include <string.h>
#       include <stdlib.h>
#       include <limits.h>
#endif

using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

TcpWorker::TcpWorker() : m_requestHandler(NULL)
{
#ifdef _WIN32
  m_thread = NULL;
  m_threadId = 0;
#else
  m_thread = 0;
  m_threadId = 0;
#endif
}

TcpWorker::~TcpWorker()
{
}

bool TcpWorker::accept(int listenSocketNo)
{
  socklen_t len = sizeof(m_address);

  m_socketNo = ::accept(listenSocketNo, (sockaddr*) &m_address, &len);
  if (m_socketNo < 0)
    return false;

  return true;
}

void TcpWorker::stop()
{
#ifdef _WIN32
  closesocket(m_socketNo);
#else
  close(m_socketNo);
#endif

#ifdef _WIN32
  if (m_thread != INVALID_HANDLE_VALUE)
  {
    WaitForSingleObject(m_thread, INFINITE);
    CloseHandle(m_thread);
    m_thread = INVALID_HANDLE_VALUE;
  }
#else
  void *status;

  if (m_thread != 0)
  {
    pthread_join(m_threadId, &status);
    m_thread = 0;
  }
#endif

}

bool TcpWorker::start()
{
  std::cout << "new socket:  " << this->m_socketNo << std::endl;

#ifdef _WIN32
  m_thread = CreateThread(NULL, 
                          0, 
                          (LPTHREAD_START_ROUTINE)thread, 
                          (LPVOID)this, 
                          0, 
                          &m_threadId);

#else
  typedef void* (*LPTHREAD_START_ROUTINE)(void *);
  m_thread = pthread_create(&m_threadId, 
                            NULL, 
                            (LPTHREAD_START_ROUTINE)thread,
                            (void*)this);
#endif

  return true;
}

int TcpWorker::send(const char *buf, int len)
{
  int sent;
  int n;
  int k;

  sent = 0;
  while (sent < len) 
  {
    /* How many bytes we send in this iteration */
    k = len - sent > INT_MAX ? INT_MAX : (int) (len - sent);

    n = ::send(m_socketNo, buf + sent, k, 0);
    if (n < 0)
    { 
      sent = -1;
      break;
    }

    sent += n;
  }

  return sent;
}

void TcpWorker::setRequestHandler(TcpRequestHandler *handler)
{
  m_requestHandler = handler; 
}

//---------------------------------------------------------------------------------------------
// Private Static API
//---------------------------------------------------------------------------------------------

void * TcpWorker::thread(TcpWorker *pWorker)
{
  bool loop = true;

  char buf[1024 * 4];
  int buf_len = 1024 * 4;

  while (loop)
  {
    memset(buf, 0, buf_len);  
    int ret = ::recv(pWorker->m_socketNo, buf, buf_len, 0);

    // TO SEND: request tool large.
    if (ret == 0)
    {
      std::cout << "[ " << pWorker->m_socketNo << " ] The connection has been gracefully closed." << std::endl;
      break;
    }

    // Remote end cosed the connection
    if (ret < 0)
    {
      std::cout << "[ " << pWorker->m_socketNo << " ] client close socket!" << std::endl;
      break;
    }

    // debug
    if (ret > 0)
      std::cout << "[ " << pWorker->m_socketNo << " ] receive:" << std::endl << buf << "[ end ]" <<std::endl;

    loop = pWorker->processData(pWorker, buf, ret);
    if (! loop)
    {
      std::cout << "[ " << pWorker->m_socketNo << " ] server close socket!" << std::endl;
      break;
    }
  }

#ifdef _WIN32
  closesocket(pWorker->m_socketNo);
#else
  close(pWorker->m_socketNo);
#endif

  return NULL;
}

//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------

bool TcpWorker::processData(TcpWorker *pWorker, const char *data, int length)
{
  if (pWorker->m_requestHandler)
    return pWorker->m_requestHandler->handleRequest(pWorker, data, length);

  return false;
}
