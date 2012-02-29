/*
 * TcpWorker.h
 *
 * A tiny tcp server framework
 *
 * jxfengzi@gmail.com
 *
 * 2011-8-9
 *
 */

#ifndef __TCP_WORKER_H__
#define __TCP_WORKER_H__


#ifdef _WIN32
#   include <winsock.h>
#else
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <pthread.h>
#endif

namespace MiniHttpServer
{

  class TcpRequestHandler;

  class TcpWorker
  {
  public:
    TcpWorker();
    virtual ~TcpWorker();

    bool accept(int listenSocketNo);
    bool start();
    void stop();

    void setRequestHandler(TcpRequestHandler *handler);
    int send(const char *buf, int len);

  private:
    static void *thread(TcpWorker *pWorker);
    bool processData(TcpWorker *pWorker, const char *data, int length);

  private:
    int                    m_socketNo;
    struct sockaddr_in     m_address;
    TcpRequestHandler     *m_requestHandler;

#ifdef _WIN32
    void *                m_thread;
    unsigned long         m_threadId;
#else
    int                   m_thread;
    pthread_t             m_threadId;
#endif
  };

}




#endif // __TCP_WORKER_H__