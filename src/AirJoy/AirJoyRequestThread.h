/* 
 * AirJoyRequestThread.h
 *
 * jxfengzi@gmail.com
 * 2012-2-28
 *
 */

#ifndef __AIRJOY_REQUEST_THREAD_H__
#define __AIRJOY_REQUEST_THREAD_H__

#include <queue>

#include "AirThread.h"
#include "AirJoyMessage.h"
#include "AirJoyCommon.h"

#include "MiniNotify.h"

using namespace miniipc;

namespace airjoy
{

  class AirJoyRequest;
  class MiniNotify;

  class AirJoyRequestThread : public AirThread
  {
  public:
    AirJoyRequestThread(AirJoyRequest *request);
    ~AirJoyRequestThread();

    virtual void run();
    void notifyThreadWillExit(void);
    
    AirJoySessionId pushToQueue(const std::string &ip, int port, AirJoyMessage *message);

  private:
    AirJoySessionId genNewSessionId(void);

  private:
    AirJoyRequest *m_request;

    // 请求队列
    CriticalSection             m_criticalSection;
    typedef struct _AirRequestInfo
    {
      AirJoySessionId        sessionId;
      AirJoyMessage         *message;
      std::string            ip;
      int                    port;
    } AirRequestInfo;

    typedef std::queue<AirRequestInfo *> AirRequestQ;
    AirRequestQ                        m_requestQ;
    unsigned int                       m_queueMaxSize;

    // 当消息队列里有发送数据时，通知发送线程
    miniipc::MiniNotify               *m_miniNotify;
  };

}

#endif // __AIRJOY_REQUEST_THREAD_H__