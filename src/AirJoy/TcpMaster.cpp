/*
 * TcpMaster.cpp
 *
 * A tiny tcp server framework
 *
 * jxfengzi@gmail.com
 *
 * 2011-8-9
 *
 */

#include "TcpMaster.h"
#include "TcpWorker.h"

#ifdef _WIN32
    typedef int socklen_t;
#else
#   include <string.h>
#   include <pthread.h>
#   include <unistd.h>
#   include <signal.h>
#endif

using namespace airjoy;

//---------------------------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------------------------

TcpMaster::TcpMaster() : m_isInit(false), m_port(0)
{
#ifdef _WIN32
  m_thread = NULL;
  m_threadId = 0;
#else
  m_thread = 0;
  m_threadId = 0;
#endif

  m_isRunning = false;
}

TcpMaster::~TcpMaster()
{
  this->stop();
}


bool TcpMaster::start()
{
  if (m_isRunning)
    return false;

  m_isRunning = true;

  init();

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

void TcpMaster::stop()
{
  this->stopWorkers();
  this->stopMaster();

  m_isRunning = false;
}

void TcpMaster::setPort(int port) 
{ 
  m_port = port; 
}

int TcpMaster::port(void)
{
  if (! m_isInit)
    return 0;

  if (m_port == 0)
  {
    struct sockaddr_in saddr_in;
    socklen_t size = sizeof(struct sockaddr_in);

    int err = getsockname(m_socketNo, (struct sockaddr*) &saddr_in, &size);
    if (! err)
      m_port = ntohs(saddr_in.sin_port);
  }

  return m_port;
}

bool TcpMaster::isRunning(void)
{
  return m_isRunning;
}

int TcpMaster::numberOfTcpConnections()
{
  return m_tcpWorkerList.size();
}

void TcpMaster::setRequestHandler(TcpRequestHandler *handler) 
{
  m_requestHandler = handler; 
}

int TcpMaster::send(TcpWorker *worker, const char *buf, int len)
{
  return worker->send(buf, len);
}

//---------------------------------------------------------------------------------------------
// Private Static API
//---------------------------------------------------------------------------------------------

void * TcpMaster::thread(TcpMaster *pMaster)
{
  //std::cout << "HttpMaster::thread" << std::endl;

  TcpWorker *pWorker = NULL; // worker在释放链表之前释放

  while (1)
  {
    pWorker = new TcpWorker();
    if (pWorker == NULL)
      break;

    if (pMaster->m_requestHandler)
      pWorker->setRequestHandler(pMaster->m_requestHandler);

    //std::cout << "prepare accept new socket ... " << std::endl;

    if (! pWorker->accept(pMaster->m_socketNo))
      break;

    pMaster->m_tcpWorkerList.push_back(pWorker);

    pWorker->start();
  }

  return NULL;
}

//---------------------------------------------------------------------------------------------
// Private API
//---------------------------------------------------------------------------------------------

bool TcpMaster::init()
{
  if (m_isInit)
    return true;

  if (! initSocket())
    return false;

  int ret = 0;

  m_socketNo = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  memset(&m_address, 0, sizeof(m_address));
  m_address.sin_family      = AF_INET;
  m_address.sin_addr.s_addr = htonl(INADDR_ANY);
  m_address.sin_port        = htons(m_port);
  ret = ::bind(m_socketNo, (struct sockaddr *)&m_address, sizeof(m_address));
  if (ret < 0)
  {
    //DWORD error = ::GetLastError();
    //std::cout << "bind: " << error << std::endl;
    return false;
  }

  m_isInit = true;

  //std::cout << "Listen TCP port: " << this->port() << std::endl;

  ::listen(m_socketNo, 5);

  return true;
}

bool TcpMaster::initSocket()
{
#ifdef WIN32
  WORD wVersionRequested;
  WSADATA wsaData;
  wVersionRequested = MAKEWORD( 2, 0 );

  if (WSAStartup(wVersionRequested, &wsaData) != 0)
    return false;
#else
  // Ignore SIGPIPE signal, so if browser cancels the request, it won't kill the whole process.
  (void) signal(SIGPIPE, SIG_IGN);
#endif

  return true;
}

void TcpMaster::stopWorkers()
{
  TcpWorkerList::iterator iter;
  for (iter = m_tcpWorkerList.begin(); iter != m_tcpWorkerList.end(); ++iter)
  {
    TcpWorker *pWorker = *iter;
    pWorker->stop();

    delete pWorker;
  }
  m_tcpWorkerList.clear();
}

void TcpMaster::stopMaster()
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
