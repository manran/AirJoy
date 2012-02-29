/*
 * TcpMaster.h
 *
 * A tiny tcp server framework
 *
 * jxfengzi@gmail.com
 *
 * 2011-8-9
 *
 */

#ifndef __TCP_MASTER_H__
#define __TCP_MASTER_H__


#ifdef _WIN32
#   include <winsock.h>
#else
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <pthread.h>
#endif

#include <list>

namespace MiniHttpServer
{

  class TcpWorker;
  class TcpRequestHandler;
  
  class TcpMaster
  {
  public:
    TcpMaster();
    virtual ~TcpMaster();

    bool start();
    void stop();
    void setPort(int port);
    int port(void);
    bool isRunning(void);
    int numberOfTcpConnections(void);

    void setRequestHandler(TcpRequestHandler *handler);

    int send(TcpWorker *worker, const char *buf, int len);

  private:
    bool init();
    bool initSocket();
    void stopWorkers();
    void stopMaster();
    static void *thread(TcpMaster *pMaster);

  private:
    bool                m_isInit;
    int                 m_port;

    int                 m_socketNo;
    struct sockaddr_in  m_address;

    TcpRequestHandler  *m_requestHandler;

    typedef std::list<TcpWorker *>  TcpWorkerList;
    TcpWorkerList       m_tcpWorkerList;

#ifdef _WIN32
    void *              m_thread;
    unsigned long       m_threadId;
#else
    int                 m_thread;
    pthread_t           m_threadId;
#endif

    bool                m_isRunning;
  };


}

#endif // __TCP_MASTER_H__