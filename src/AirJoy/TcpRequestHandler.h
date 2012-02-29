/*
 * TcpRequestHandler.h
 *
 * A tiny tcp server framework
 *
 * jxfengzi@gmail.com
 *
 * 2011-8-9
 *
 */

#ifndef __TCP_REQUEST_HANDLER_H__
#define __TCP_REQUEST_HANDLER_H__

namespace airjoy
{

  class TcpWorker;
 
  class TcpRequestHandler
  {
  public:
    TcpRequestHandler() {}
    virtual ~TcpRequestHandler() {}

    // 返回false，则关闭此连接。
    virtual bool handleRequest(TcpWorker *pWorker, const char *data, int length) = 0;
  };

}

#endif  // __TCP_REQUEST_HANDLER_H__